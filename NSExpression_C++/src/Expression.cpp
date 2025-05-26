#include "Expression.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <vector>

std::vector<double> EquationNode::solveNonLinear(const std::string& var, const std::unordered_map<std::string, double>& variables) const {
    auto f = [&](double x) {
        std::unordered_map<std::string, double> vars = variables;
        vars[var] = x;
        return left->evaluate(vars) - right->evaluate(vars);
    };
    auto df = [&](double x) {
        const double h = 1e-6;
        return (f(x + h) - f(x - h)) / (2 * h);
    };
    std::vector<double> solutions;
    const double tolerance = 1e-6;
    const int max_iterations = 100;
    for (double x0 = -10.0; x0 <= 10.0; x0 += 1.0) { // Range [-10, 10]
        double x = x0;
        for (int i = 0; i < max_iterations; ++i) {
            double fx = f(x);
            if (std::abs(fx) < tolerance) {
                bool unique = true;
                for (double sol : solutions) {
                    if (std::abs(x - sol) < tolerance) {
                        unique = false;
                        break;
                    }
                }
                if (unique) solutions.push_back(x);
                break;
            }
            double dfx = df(x);
            if (std::abs(dfx) < 1e-10) break;
            x = x - fx / dfx;
        }
    }
    return solutions;
}