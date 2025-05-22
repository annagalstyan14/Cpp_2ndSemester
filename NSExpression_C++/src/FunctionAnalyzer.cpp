#include "FunctionAnalyzer.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <ncurses.h>
#include <fstream>

FunctionAnalyzer::FunctionAnalyzer(const std::shared_ptr<ExpressionNode>& expr) : expr(expr) {}

std::vector<std::pair<double, double>> FunctionAnalyzer::getRestrictedDomain(const std::shared_ptr<ExpressionNode>& node) const {
    // [Unchanged code from your original FunctionAnalyzer.cpp]
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
    // [Unchanged code]
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
            if (std::abs(x) < EPSILON) continue;
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
    // [Unchanged code]
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
                return power % 2 == 1;
            }
        } else if (bin->getOp() == "/") {
            if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                        return true;
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
    // [Unchanged code]
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
                return power % 2 == 0;
            }
        } else if (bin->getOp() == "/") {
            if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                        return false;
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
    // [Unchanged code]
    auto func = std::dynamic_pointer_cast<FunctionNode>(expr);
    if (func) {
        if (func->getName() == "sin") {
            std::vector<std::pair<double, double>> intercepts;
            const double pi = 3.141592653589793;
            const int n = 3;
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
            const int n = 3;
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
            const int n = 3;
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
            return { {1.0, 0.0} };
        }
    } else if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(expr)) {
        if (bin->getOp() == "^") {
            if (auto rightConst = std::dynamic_pointer_cast<NumberNode>(bin->getRight())) {
                return { {0.0, 0.0} };
            }
        } else if (bin->getOp() == "/") {
            if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                        return {};
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
    // [Unchanged code]
    auto bin = std::dynamic_pointer_cast<BinaryOpNode>(expr);
    if (bin && bin->getOp() == "/") {
        if (auto leftConst = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
            if (auto rightVar = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                if (std::abs(leftConst->getValue() - 1.0) < EPSILON && rightVar->getName() == "x") {
                    return { 0.0, std::numeric_limits<double>::quiet_NaN() };
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

void FunctionAnalyzer::plotNcurses(const std::string& filename) const {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int width = max_x - 2;
    int height = max_y - 2;

    auto domains = getDomain();
    double minX = std::numeric_limits<double>::infinity();
    double maxX = -std::numeric_limits<double>::infinity();
    for (const auto& domain : domains) {
        minX = std::min(minX, domain.first);
        maxX = std::max(maxX, domain.second);
    }
    minX = std::max(PLOT_MIN, minX);
    maxX = std::min(PLOT_MAX, maxX);

    double x_center = (minX + maxX) / 2.0;
    double x_range = maxX - minX;
    double zoom = 1.0;

    std::vector<std::pair<double, double>> coordinates;
    bool running = true;

    while (running) {
        clear();

        double current_minX = x_center - (x_range * zoom) / 2.0;
        double current_maxX = x_center + (x_range * zoom) / 2.0;

        double minY = std::numeric_limits<double>::infinity();
        double maxY = -std::numeric_limits<double>::infinity();
        coordinates.clear();
        std::unordered_map<std::string, double> vars;
        bool hasFiniteValues = false;

        auto func = std::dynamic_pointer_cast<FunctionNode>(expr);
        bool isTan = func && func->getName() == "tan";

        for (const auto& domain : domains) {
            double start = std::max(domain.first, current_minX);
            double end = std::min(domain.second, current_maxX);
            if (start >= end) continue;

            if (isTan) {
                const double pi = 3.141592653589793;
                double mid = (start + end) / 2;
                double k = std::round(mid / pi);
                start = std::max(start, k * pi - pi / 4);
                end = std::min(end, k * pi + pi / 4);
                if (start >= end) continue;
            }

            for (double x = start; x <= end; x += (current_maxX - current_minX) / (width * 10)) {
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
                        coordinates.emplace_back(x, y);
                        hasFiniteValues = true;
                    }
                } catch (const std::exception&) {}
            }
        }

        if (!hasFiniteValues || !std::isfinite(minY) || !std::isfinite(maxY)) {
            mvprintw(max_y / 2, (max_x - 30) / 2, "No finite values in range");
            refresh();
            int ch = getch();
            if (ch == 'q') running = false;
            continue;
        }

        double rangePadding = (maxY - minY) * 0.1;
        if (rangePadding == 0) rangePadding = 1.0;
        minY -= rangePadding;
        maxY += rangePadding;

        int xAxis = std::max(0, std::min(height - 1, static_cast<int>((0 - minY) / (maxY - minY) * (height - 1))));
        int yAxis = std::max(0, std::min(width - 1, static_cast<int>((0 - current_minX) / (current_maxX - current_minX) * (width - 1))));

        for (int i = 0; i < height; ++i) {
            mvaddch(i + 1, yAxis + 1, '|');
        }
        for (int j = 0; j < width; ++j) {
            mvaddch(xAxis + 1, j + 1, '-');
        }
        mvaddch(xAxis + 1, yAxis + 1, '+');

        for (const auto& coord : coordinates) {
            double x = coord.first;
            double y = coord.second;
            if (std::isfinite(y) && y >= minY && y <= maxY) {
                int col = static_cast<int>((x - current_minX) / (current_maxX - current_minX) * (width - 1));
                int row = height - 1 - static_cast<int>((y - minY) / (maxY - minY) * (height - 1));
                if (row >= 0 && row < height && col >= 0 && col < width) {
                    mvaddch(row + 1, col + 1, '*');
                }
            }
        }

        mvprintw(0, 0, "X: [%.2f, %.2f], Y: [%.2f, %.2f]", current_minX, current_maxX, minY, maxY);
        mvprintw(max_y - 1, 0, "Arrows: Pan, +/-: Zoom, s: Save, q: Quit");
        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_LEFT:
                x_center -= x_range * zoom * 0.1;
                break;
            case KEY_RIGHT:
                x_center += x_range * zoom * 0.1;
                break;
            case KEY_UP:
                zoom *= 0.9;
                break;
            case KEY_DOWN:
                zoom /= 0.9;
                break;
            case '+':
                zoom *= 0.9;
                break;
            case '-':
                zoom /= 0.9;
                break;
            case 's':
                if (!filename.empty()) {
                    std::ofstream ofs(filename);
                    if (ofs) {
                        ofs << std::fixed << std::setprecision(2);
                        ofs << "x y\n";
                        for (const auto& coord : coordinates) {
                            ofs << coord.first << " " << coord.second << "\n";
                        }
                        ofs.close();
                    }
                }
                break;
            case 'q':
                running = false;
                break;
        }
    }

    endwin();

    if (!filename.empty()) {
        std::ofstream ofs(filename);
        if (ofs) {
            ofs << std::fixed << std::setprecision(2);
            ofs << "x y\n";
            for (const auto& coord : coordinates) {
                ofs << coord.first << " " << coord.second << "\n";
            }
            ofs.close();
        }
    }
}