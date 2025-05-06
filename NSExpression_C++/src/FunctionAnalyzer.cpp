#include "FunctionAnalyzer.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <iomanip>

std::pair<double, double> FunctionAnalyzer::getRestrictedDomain(const std::shared_ptr<ExpressionNode>& node) const {
    if (auto func = std::dynamic_pointer_cast<FunctionNode>(node)) {
        if (func->getName() == "sqrt" || func->getName() == "cbrt") {
            return { 0.0, std::numeric_limits<double>::infinity() }; // sqrt(x) requires x >= 0
        }
        if (func->getName() == "log" || func->getName() == "ln" || func->getName() == "log10" || func->getName() == "log2") {
            return { 1e-10, std::numeric_limits<double>::infinity() }; // log(x) requires x > 0
        }
        if (func->getName() == "tan") {
            // tan(x) is undefined at x = π/2 + kπ; approximate with a safe range
            return { -1.5, 1.5 }; // Restrict to avoid singularities
        }
        if (func->getArgs().size() == 1) {
            return getRestrictedDomain(func->getArgs()[0]); // Recurse into arguments
        }
    } else if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(node)) {
        auto leftDomain = getRestrictedDomain(bin->getLeft());
        auto rightDomain = getRestrictedDomain(bin->getRight());
        if (bin->getOp() == "/") {
            // For division, exclude points where denominator is zero
            std::unordered_map<std::string, double> vars;
            for (double x = PLOT_MIN; x <= PLOT_MAX; x += PLOT_STEP) {
                vars["x"] = x;
                try {
                    double denom = bin->getRight()->evaluate(vars);
                    if (std::abs(denom) < EPSILON) {
                        return { PLOT_MIN, x - EPSILON }; // Restrict domain before zero
                    }
                } catch (const std::exception&) {
                    return { PLOT_MIN, x - EPSILON };
                }
            }
        }
        // Intersect domains
        return { std::max(leftDomain.first, rightDomain.first), std::min(leftDomain.second, rightDomain.second) };
    } else if (auto unary = std::dynamic_pointer_cast<UnaryOpNode>(node)) {
        return getRestrictedDomain(unary->getOperand());
    }
    return { -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() };
}

std::pair<double, double> FunctionAnalyzer::getDomain() const {
    return getRestrictedDomain(expr);
}

std::pair<double, double> FunctionAnalyzer::getRange() const {
    auto domain = getDomain();
    double minX = std::max(PLOT_MIN, domain.first);
    double maxX = std::min(PLOT_MAX, domain.second);
    std::unordered_map<std::string, double> vars;
    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();
    for (double x = minX; x <= maxX; x += PLOT_STEP) {
        vars["x"] = x;
        try {
            double y = expr->evaluate(vars);
            minY = std::min(minY, y);
            maxY = std::max(maxY, y);
        } catch (const std::exception&) {
            // Skip undefined points
        }
    }
    return { minY, maxY };
}

bool FunctionAnalyzer::isOdd() const {
    auto domain = getDomain();
    double minX = std::max(PLOT_MIN, domain.first);
    double maxX = std::min(PLOT_MAX, domain.second);
    std::unordered_map<std::string, double> vars;
    for (double x = minX; x <= maxX; x += PLOT_STEP) {
        vars["x"] = x;
        double f_x = expr->evaluate(vars);
        vars["x"] = -x;
        if (-x < domain.first || -x > domain.second) continue; // Skip if -x is outside domain
        double f_neg_x = expr->evaluate(vars);
        if (std::abs(f_x + f_neg_x) > EPSILON) return false;
    }
    return true;
}

bool FunctionAnalyzer::isEven() const {
    auto domain = getDomain();
    double minX = std::max(PLOT_MIN, domain.first);
    double maxX = std::min(PLOT_MAX, domain.second);
    std::unordered_map<std::string, double> vars;
    for (double x = minX; x <= maxX; x += PLOT_STEP) {
        vars["x"] = x;
        double f_x = expr->evaluate(vars);
        vars["x"] = -x;
        if (-x < domain.first || -x > domain.second) continue;
        double f_neg_x = expr->evaluate(vars);
        if (std::abs(f_x - f_neg_x) > EPSILON) return false;
    }
    return true;
}

bool FunctionAnalyzer::isSymmetric() const {
    return isEven(); // Symmetry about y-axis for even functions
}

std::vector<double> FunctionAnalyzer::getXAxisIntercepts() const {
    auto domain = getDomain();
    double minX = std::max(PLOT_MIN, domain.first);
    double maxX = std::min(PLOT_MAX, domain.second);
    std::vector<double> intercepts;
    std::unordered_map<std::string, double> vars;
    for (double x = minX; x <= maxX; x += PLOT_STEP / 10) {
        vars["x"] = x;
        try {
            double y = expr->evaluate(vars);
            if (std::abs(y) < EPSILON) {
                intercepts.push_back(x);
            } else {
                vars["x"] = x - PLOT_STEP / 10;
                double y_prev = expr->evaluate(vars);
                if (y * y_prev < 0) { // Sign change indicates a root
                    intercepts.push_back(x - PLOT_STEP / 20); // Approximate
                }
            }
        } catch (const std::exception&) {
            // Skip undefined points
        }
    }
    return intercepts;
}

double FunctionAnalyzer::getYAxisIntercept() const {
    auto domain = getDomain();
    if (0.0 < domain.first || 0.0 > domain.second) {
        return std::numeric_limits<double>::quiet_NaN(); // Y-intercept undefined
    }
    std::unordered_map<std::string, double> vars;
    vars["x"] = 0.0;
    try {
        return expr->evaluate(vars);
    } catch (const std::exception&) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

void FunctionAnalyzer::plotAscii(std::ostream& os, int width, int height) const {
    auto domain = getDomain();
    double minX = std::max(PLOT_MIN, domain.first);
    double maxX = std::min(PLOT_MAX, domain.second);
    auto range = getRange();
    double minY = range.first;
    double maxY = range.second;

    // Handle case where range is invalid or infinite
    if (!std::isfinite(minY) || !std::isfinite(maxY)) {
        os << "Cannot plot: Range is undefined or infinite\n";
        return;
    }

    // Initialize grid
    std::vector<std::vector<char>> grid(height, std::vector<char>(width, ' '));
    int xAxis = std::max(0, std::min(height - 1, static_cast<int>((0 - minY) / (maxY - minY) * (height - 1))));
    int yAxis = std::max(0, std::min(width - 1, static_cast<int>((0 - minX) / (maxX - minX) * (width - 1))));

    // Plot axes
    for (int i = 0; i < height; ++i) {
        grid[i][yAxis] = '|';
    }
    for (int j = 0; j < width; ++j) {
        grid[xAxis][j] = '-';
    }
    grid[xAxis][yAxis] = '+';

    // Evaluate function and plot points
    std::unordered_map<std::string, double> vars;
    for (double x = minX; x <= maxX; x += (maxX - minX) / width) {
        vars["x"] = x;
        try {
            double y = expr->evaluate(vars);
            if (std::isfinite(y) && y >= minY && y <= maxY) {
                int col = static_cast<int>((x - minX) / (maxX - minX) * (width - 1));
                int row = height - 1 - static_cast<int>((y - minY) / (maxY - minY) * (height - 1));
                if (row >= 0 && row < height && col >= 0 && col < width) {
                    grid[row][col] = '*';
                }
            }
        } catch (const std::exception&) {
            // Skip undefined points
        }
    }

    // Print grid
    os << "y\n";
    for (int i = 0; i < height; ++i) {
        os << "| ";
        for (int j = 0; j < width; ++j) {
            os << grid[i][j];
        }
        os << "\n";
    }
    os << "  ";
    for (int j = 0; j < width; ++j) {
        os << "-";
    }
    os << "> x\n";
    os << "X: [" << minX << ", " << maxX << "], Y: [" << minY << ", " << maxY << "]\n";
}