#include "FunctionAnalyzer.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <iomanip>

std::vector<std::pair<double, double>> FunctionAnalyzer::getRestrictedDomain(const std::shared_ptr<ExpressionNode>& node) const {
    if (auto func = std::dynamic_pointer_cast<FunctionNode>(node)) {
        if (func->getName() == "sqrt" || func->getName() == "cbrt") {
            return { {0.0, std::numeric_limits<double>::infinity()} };
        }
        if (func->getName() == "log" || func->getName() == "ln" || func->getName() == "log10" || func->getName() == "log2") {
            return { {1e-10, std::numeric_limits<double>::infinity()} };
        }
        if (func->getName() == "tan") {
            std::vector<std::pair<double, double>> intervals;
            const double pi = 3.141592653589793;
            for (int k = -3; k <= 3; ++k) {
                double start = k * pi - pi / 2 + EPSILON;
                double end = k * pi + pi / 2 - EPSILON;
                if (end >= PLOT_MIN && start <= PLOT_MAX) {
                    intervals.emplace_back(std::max(start, PLOT_MIN), std::min(end, PLOT_MAX));
                }
            }
            return intervals;
        }
        if (func->getName() == "asin" || func->getName() == "acos") {
            return { {-1.0, 1.0} };
        }
        if (func->getName() == "atan" || func->getName() == "sin" || func->getName() == "cos" || func->getName() == "exp") {
            return { {-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()} };
        }
        if (func->getArgs().size() == 1) {
            return getRestrictedDomain(func->getArgs()[0]);
        }
    } else if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(node)) {
        auto leftDomain = getRestrictedDomain(bin->getLeft());
        auto rightDomain = getRestrictedDomain(bin->getRight());
        if (bin->getOp() == "^") {
            if (auto rightConst = std::dynamic_pointer_cast<NumberNode>(bin->getRight())) {
                int power = static_cast<int>(rightConst->getValue());
                if (power % 2 == 0) {
                    return { {0.0, std::numeric_limits<double>::infinity()} }; // x^2, x^4
                } else {
                    return { {-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()} }; // x^3, x^5
                }
            }
        } else if (bin->getOp() == "/") {
            if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                        return { {-std::numeric_limits<double>::infinity(), -EPSILON}, {EPSILON, std::numeric_limits<double>::infinity()} }; // 1/x
                    }
                }
            }
            // General division: exclude denominator zeros numerically
            std::vector<std::pair<double, double>> intervals;
            double last = PLOT_MIN;
            std::unordered_map<std::string, double> vars;
            bool open = true;
            for (double x = PLOT_MIN; x <= PLOT_MAX + EPSILON; x += PLOT_STEP) {
                vars["x"] = x;
                try {
                    double denom = bin->getRight()->evaluate(vars);
                    if (std::abs(denom) < EPSILON && open) {
                        if (x - last > EPSILON) {
                            intervals.emplace_back(last, x - EPSILON);
                        }
                        last = x + EPSILON;
                        open = false;
                    } else if (std::abs(denom) >= EPSILON && !open) {
                        open = true;
                    }
                } catch (const std::exception&) {
                    if (open) {
                        if (x - last > EPSILON) {
                            intervals.emplace_back(last, x - EPSILON);
                        }
                        last = x + EPSILON;
                        open = false;
                    }
                }
            }
            if (open && PLOT_MAX - last > EPSILON) {
                intervals.emplace_back(last, PLOT_MAX);
            }
            return intervals;
        }
        // Intersect domains
        std::vector<std::pair<double, double>> result;
        for (const auto& l : leftDomain) {
            for (const auto& r : rightDomain) {
                double start = std::max(l.first, r.first);
                double end = std::min(l.second, r.second);
                if (start < end - EPSILON) {
                    result.emplace_back(start, end);
                }
            }
        }
        return result;
    } else if (auto unary = std::dynamic_pointer_cast<UnaryOpNode>(node)) {
        return getRestrictedDomain(unary->getOperand());
    } else if (auto var = std::dynamic_pointer_cast<VariableNode>(node)) {
        return { {-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()} };
    }
    return { {-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()} };
}

std::vector<std::pair<double, double>> FunctionAnalyzer::getDomain() const {
    return getRestrictedDomain(expr);
}

std::pair<double, double> FunctionAnalyzer::getRange() const {
    auto func = std::dynamic_pointer_cast<FunctionNode>(expr);
    if (func) {
        if (func->getName() == "sin" || func->getName() == "cos") {
            return { -1.0, 1.0 };
        }
        if (func->getName() == "tan" || func->getName() == "atan") {
            return { -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() };
        }
        if (func->getName() == "asin") {
            return { -3.141592653589793 / 2, 3.141592653589793 / 2 };
        }
        if (func->getName() == "acos") {
            return { 0.0, 3.141592653589793 };
        }
        if (func->getName() == "sqrt") {
            return { 0.0, std::numeric_limits<double>::infinity() };
        }
        if (func->getName() == "cbrt") {
            return { -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() };
        }
        if (func->getName() == "exp") {
            return { 0.0, std::numeric_limits<double>::infinity() };
        }
        if (func->getName() == "log" || func->getName() == "ln" || func->getName() == "log10" || func->getName() == "log2") {
            return { -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() };
        }
    } else if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(expr)) {
        if (bin->getOp() == "^") {
            if (auto rightConst = std::dynamic_pointer_cast<NumberNode>(bin->getRight())) {
                int power = static_cast<int>(rightConst->getValue());
                if (power % 2 == 0) {
                    return { 0.0, std::numeric_limits<double>::infinity() }; // x^2, x^4
                } else {
                    return { -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() }; // x^3, x^5
                }
            }
        } else if (bin->getOp() == "/") {
            if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                        return { -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() }; // 1/x
                    }
                }
            }
        }
    }

    auto domains = getDomain();
    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();
    std::unordered_map<std::string, double> vars;
    for (const auto& domain : domains) {
        double minX = domain.first;
        double maxX = domain.second;
        for (double x = minX; x <= maxX; x += PLOT_STEP) {
            if (std::abs(x) < EPSILON) continue; // Avoid division by zero
            vars["x"] = x;
            try {
                double y = expr->evaluate(vars);
                if (std::isfinite(y)) {
                    minY = std::min(minY, y);
                    maxY = std::max(maxY, y);
                }
            } catch (const std::exception&) {
                // Skip undefined points
            }
        }
    }
    if (!std::isfinite(minY) || !std::isfinite(maxY)) {
        return { -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() };
    }
    return { minY, maxY };
}

bool FunctionAnalyzer::isOdd() const {
    auto func = std::dynamic_pointer_cast<FunctionNode>(expr);
    if (func) {
        if (func->getName() == "sin" || func->getName() == "tan" || func->getName() == "cbrt" ||
            func->getName() == "asin" || func->getName() == "atan") {
            return true;
        }
        if (func->getName() == "cos" || func->getName() == "acos" || 
            func->getName() == "log" || func->getName() == "ln" ||
            func->getName() == "log10" || func->getName() == "log2" || 
            func->getName() == "exp" || func->getName() == "sqrt") {
            return false;
        }
    } else if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(expr)) {
        if (bin->getOp() == "^") {
            if (auto rightConst = std::dynamic_pointer_cast<NumberNode>(bin->getRight())) {
                int power = static_cast<int>(rightConst->getValue());
                return power % 2 == 1; // x^3, x^5 are odd
            }
        } else if (bin->getOp() == "/") {
            if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                        return true; // 1/x is odd
                    }
                }
            }
        }
    }

    auto domains = getDomain();
    if (!std::isinf(domains[0].first) && domains[0].first >= 0) {
        return false;
    }
    std::unordered_map<std::string, double> vars;
    bool hasValidPoint = false;
    for (const auto& domain : domains) {
        double minX = domain.first;
        double maxX = domain.second;
        for (double x = minX; x <= maxX; x += PLOT_STEP) {
            if (std::abs(x) < EPSILON) continue;
            vars["x"] = x;
            double f_x;
            try {
                f_x = expr->evaluate(vars);
            } catch (const std::exception&) {
                continue;
            }
            vars["x"] = -x;
            bool inDomain = false;
            for (const auto& d : domains) {
                if (-x >= d.first - EPSILON && -x <= d.second + EPSILON) {
                    inDomain = true;
                    break;
                }
            }
            if (!inDomain) continue;
            double f_neg_x;
            try {
                f_neg_x = expr->evaluate(vars);
            } catch (const std::exception&) {
                continue;
            }
            if (std::abs(f_x + f_neg_x) > EPSILON) {
                return false;
            }
            hasValidPoint = true;
        }
    }
    return hasValidPoint;
}

bool FunctionAnalyzer::isEven() const {
    auto func = std::dynamic_pointer_cast<FunctionNode>(expr);
    if (func) {
        if (func->getName() == "cos") {
            return true;
        }
        if (func->getName() == "sin" || func->getName() == "tan" || func->getName() == "asin" || 
            func->getName() == "acos" || func->getName() == "atan" || func->getName() == "log" ||
            func->getName() == "ln" || func->getName() == "log10" || func->getName() == "log2" ||
            func->getName() == "exp" || func->getName() == "sqrt" || func->getName() == "cbrt") {
            return false;
        }
    } else if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(expr)) {
        if (bin->getOp() == "^") {
            if (auto rightConst = std::dynamic_pointer_cast<NumberNode>(bin->getRight())) {
                int power = static_cast<int>(rightConst->getValue());
                return power % 2 == 0; // x^2, x^4 are even
            }
        } else if (bin->getOp() == "/") {
            if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                        return false; // 1/x is not even
                    }
                }
            }
        }
    }

    auto domains = getDomain();
    if (!std::isinf(domains[0].first) && domains[0].first >= 0) {
        return false;
    }
    std::unordered_map<std::string, double> vars;
    bool hasValidPoint = false;
    for (const auto& domain : domains) {
        double minX = domain.first;
        double maxX = domain.second;
        for (double x = minX; x <= maxX; x += PLOT_STEP) {
            if (std::abs(x) < EPSILON) continue;
            vars["x"] = x;
            double f_x;
            try {
                f_x = expr->evaluate(vars);
            } catch (const std::exception&) {
                continue;
            }
            vars["x"] = -x;
            bool inDomain = false;
            for (const auto& d : domains) {
                if (-x >= d.first - EPSILON && -x <= d.second + EPSILON) {
                    inDomain = true;
                    break;
                }
            }
            if (!inDomain) continue;
            double f_neg_x;
            try {
                f_neg_x = expr->evaluate(vars);
            } catch (const std::exception&) {
                continue;
            }
            if (std::abs(f_x - f_neg_x) > EPSILON) {
                return false;
            }
            hasValidPoint = true;
        }
    }
    return hasValidPoint;
}

bool FunctionAnalyzer::isSymmetric() const {
    return isEven();
}

std::vector<std::pair<double, double>> FunctionAnalyzer::getXAxisIntercepts() const {
    auto func = std::dynamic_pointer_cast<FunctionNode>(expr);
    if (func) {
        if (func->getName() == "sin") {
            std::vector<std::pair<double, double>> intercepts;
            const double pi = 3.141592653589793;
            const int n = 3; // -3π to 3π
            for (int k = -n; k <= n; ++k) {
                intercepts.emplace_back(k * pi, 0.0);
            }
            std::sort(intercepts.begin(), intercepts.end(),
                      [](const auto& a, const auto& b) { return a.first < b.first; });
            return intercepts;
        }
        if (func->getName() == "cos") {
            std::vector<std::pair<double, double>> intercepts;
            const double pi = 3.141592653589793;
            const int n = 3; // -π/2 + kπ
            for (int k = -n; k <= n; ++k) {
                intercepts.emplace_back(pi / 2 + k * pi, 0.0);
            }
            std::sort(intercepts.begin(), intercepts.end(),
                      [](const auto& a, const auto& b) { return a.first < b.first; });
            return intercepts;
        }
        if (func->getName() == "tan") {
            std::vector<std::pair<double, double>> intercepts;
            const double pi = 3.141592653589793;
            const int n = 3; // kπ
            for (int k = -n; k <= n; ++k) {
                intercepts.emplace_back(k * pi, 0.0);
            }
            std::sort(intercepts.begin(), intercepts.end(),
                      [](const auto& a, const auto& b) { return a.first < b.first; });
            return intercepts;
        }
        if (func->getName() == "asin" || func->getName() == "atan") {
            return { {0.0, 0.0} };
        }
        if (func->getName() == "acos") {
            return { {1.0, 0.0} };
        }
        if (func->getName() == "sqrt" || func->getName() == "cbrt") {
            return { {0.0, 0.0} };
        }
        if (func->getName() == "exp") {
            return {};
        }
        if (func->getName() == "log" || func->getName() == "ln" || func->getName() == "log10" || func->getName() == "log2") {
            return { {1.0, 0.0} }; // log(1) = 0
        }
    } else if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(expr)) {
        if (bin->getOp() == "^") {
            if (auto rightConst = std::dynamic_pointer_cast<NumberNode>(bin->getRight())) {
                return { {0.0, 0.0} }; // x^n = 0 at x = 0
            }
        } else if (bin->getOp() == "/") {
            if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                        return {}; // 1/x has no x-intercepts
                    }
                }
            }
        }
    }

    auto domains = getDomain();
    std::vector<std::pair<double, double>> intercepts;
    std::unordered_map<std::string, double> vars;
    for (const auto& domain : domains) {
        double minX = domain.first;
        double maxX = domain.second;
        for (double x = minX; x <= maxX; x += PLOT_STEP / 10) {
            if (std::abs(x) < EPSILON) continue;
            vars["x"] = x;
            try {
                double y = expr->evaluate(vars);
                if (std::abs(y) < EPSILON) {
                    intercepts.emplace_back(x, 0.0);
                } else {
                    vars["x"] = x - PLOT_STEP / 10;
                    try {
                        double y_prev = expr->evaluate(vars);
                        if (y * y_prev < 0 && std::isfinite(y) && std::isfinite(y_prev)) {
                            intercepts.emplace_back(x - PLOT_STEP / 20, 0.0);
                        }
                    } catch (const std::exception&) {
                        // Skip undefined points
                    }
                }
            } catch (const std::exception&) {
                // Skip undefined points
            }
        }
    }
    std::sort(intercepts.begin(), intercepts.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });
    return intercepts;
}

std::pair<double, double> FunctionAnalyzer::getYAxisIntercept() const {
    auto bin = std::dynamic_pointer_cast<BinaryOpNode>(expr);
    if (bin && bin->getOp() == "/") {
        if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
            if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                    return { 0.0, std::numeric_limits<double>::quiet_NaN() }; // 1/x undefined at x=0
                }
            }
        }
    }

    auto domains = getDomain();
    bool includesZero = false;
    for (const auto& domain : domains) {
        if (domain.first <= 0.0 + EPSILON && domain.second >= 0.0 - EPSILON) {
            includesZero = true;
            break;
        }
    }
    if (!includesZero) {
        return { 0.0, std::numeric_limits<double>::quiet_NaN() };
    }
    std::unordered_map<std::string, double> vars;
    vars["x"] = 0.0;
    try {
        double y = expr->evaluate(vars);
        return { 0.0, std::abs(y) < EPSILON ? 0.0 : y };
    } catch (const std::exception&) {
        return { 0.0, std::numeric_limits<double>::quiet_NaN() };
    }
}

void FunctionAnalyzer::plotAscii(std::ostream& os, int width, int height) const {
    auto domains = getDomain();
    double minX = std::numeric_limits<double>::infinity();
    double maxX = -std::numeric_limits<double>::infinity();
    for (const auto& domain : domains) {
        minX = std::min(minX, domain.first);
        maxX = std::max(maxX, domain.second);
    }
    minX = std::max(PLOT_MIN, minX);
    maxX = std::min(PLOT_MAX, maxX);

    // Compute practical range for plotting
    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();
    std::unordered_map<std::string, double> vars;
    bool hasFiniteValues = false;

    auto func = std::dynamic_pointer_cast<FunctionNode>(expr);
    bool isTan = func && func->getName() == "tan";

    for (const auto& domain : domains) {
        double start = std::max(domain.first, minX);
        double end = std::min(domain.second, maxX);
        if (start >= end) continue;

        // For tan(x), avoid asymptotes by narrowing the interval
        if (isTan) {
            const double pi = 3.141592653589793;
            double mid = (start + end) / 2;
            double k = std::round(mid / pi);
            start = std::max(start, k * pi - pi / 4); // Avoid ±π/2
            end = std::min(end, k * pi + pi / 4);
            if (start >= end) continue;
        }

        for (double x = start; x <= end; x += (maxX - minX) / (width * 10)) {
            if (std::abs(x) < EPSILON && (func && (func->getName() == "log" || func->getName() == "ln" ||
                                                  func->getName() == "log10" || func->getName() == "log2" ||
                                                  func->getName() == "1/x"))) {
                continue;
            }
            vars["x"] = x;
            try {
                double y = expr->evaluate(vars);
                if (std::isfinite(y)) {
                    minY = std::min(minY, y);
                    maxY = std::max(maxY, y);
                    hasFiniteValues = true;
                }
            } catch (const std::exception&) {
                // Skip undefined points
            }
        }
    }

    if (!hasFiniteValues || !std::isfinite(minY) || !std::isfinite(maxY)) {
        os << "Cannot plot: No finite values in the evaluated range\n";
        return;
    }

    // Add padding to the range for better visualization
    double rangePadding = (maxY - minY) * 0.1;
    if (rangePadding == 0) rangePadding = 1.0; // Avoid zero range
    minY -= rangePadding;
    maxY += rangePadding;

    std::vector<std::vector<char>> grid(height, std::vector<char>(width, ' '));
    int xAxis = std::max(0, std::min(height - 1, static_cast<int>((0 - minY) / (maxY - minY) * (height - 1))));
    int yAxis = std::max(0, std::min(width - 1, static_cast<int>((0 - minX) / (maxX - minX) * (width - 1))));

    for (int i = 0; i < height; ++i) {
        grid[i][yAxis] = '|';
    }
    for (int j = 0; j < width; ++j) {
        grid[xAxis][j] = '-';
    }
    grid[xAxis][yAxis] = '+';

    for (const auto& domain : domains) {
        double start = std::max(domain.first, minX);
        double end = std::min(domain.second, maxX);
        if (start >= end) continue;

        if (isTan) {
            const double pi = 3.141592653589793;
            double mid = (start + end) / 2;
            double k = std::round(mid / pi);
            start = std::max(start, k * pi - pi / 4);
            end = std::min(end, k * pi + pi / 4);
            if (start >= end) continue;
        }

        for (double x = start; x <= end; x += (maxX - minX) / width) {
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
    }

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
    os << "X: [" << std::fixed << std::setprecision(2) << minX << ", " << maxX << "], Y: [" 
       << std::fixed << std::setprecision(2) << minY << ", " << maxY << "]\n";
}