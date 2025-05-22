#include "Expression.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <vector>

std::vector<double> EquationNode::solveNonLinear(const std::string& var, const std::unordered_map<std::string, double>& variables) const {
    std::vector<double> solutions;
    const double h = 1e-6;
    const int max_iterations = 100;
    const double tolerance = 1e-6;

    // Define f(x) = left - right
    auto f = [&](double x) {
        std::unordered_map<std::string, double> vars = variables;
        vars[var] = x;
        return left->evaluate(vars) - right->evaluate(vars);
    };

    // Numerical derivative
    auto df = [&](double x) {
        return (f(x + h) - f(x - h)) / (2 * h);
    };

    // Try initial guesses across the domain [-10, 10]
    for (double x0 = -10.0; x0 <= 10.0; x0 += 1.0) {
        double x = x0;
        for (int i = 0; i < max_iterations; ++i) {
            double fx = f(x);
            double dfx = df(x);
            if (std::abs(dfx) < 1e-10) break; // Avoid division by zero
            double x_new = x - fx / dfx;
            if (std::abs(x_new - x) < tolerance) {
                bool is_unique = true;
                for (double sol : solutions) {
                    if (std::abs(sol - x_new) < tolerance) {
                        is_unique = false;
                        break;
                    }
                }
                if (is_unique && std::isfinite(x_new) && std::abs(f(x_new)) < tolerance) {
                    solutions.push_back(x_new);
                }
                break;
            }
            x = x_new;
        }
    }

    std::sort(solutions.begin(), solutions.end());
    return solutions;
}
