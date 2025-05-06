#ifndef FUNCTION_ANALYZER_H
#define FUNCTION_ANALYZER_H

#include "Expression.h"
#include <vector>
#include <string>
#include <iostream>

class FunctionAnalyzer {
private:
    std::shared_ptr<ExpressionNode> expr;
    static constexpr double EPSILON = 1e-6;
    static constexpr double PLOT_STEP = 0.1;
    static constexpr double PLOT_MIN = -10.0;
    static constexpr double PLOT_MAX = 10.0;
public:
    FunctionAnalyzer(std::shared_ptr<ExpressionNode> expr) : expr(expr) {}
    std::pair<double, double> getDomain() const;
    std::pair<double, double> getRestrictedDomain(const std::shared_ptr<ExpressionNode>& node) const;
    std::pair<double, double> getRange() const;
    bool isOdd() const;
    bool isEven() const;
    bool isSymmetric() const;
    std::vector<double> getXAxisIntercepts() const;
    double getYAxisIntercept() const;
    void plotAscii(std::ostream& os, int width = 80, int height = 20) const;
};

#endif