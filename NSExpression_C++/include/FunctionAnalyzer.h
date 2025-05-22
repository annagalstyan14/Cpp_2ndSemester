#ifndef FUNCTIONANALYZER_H
#define FUNCTIONANALYZER_H

#include "Parser.h"
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <memory>

class FunctionAnalyzer {
public:
    static constexpr double PLOT_MIN = -10.0;
    static constexpr double PLOT_MAX = 10.0;
    static constexpr double PLOT_STEP = 0.1;
    static constexpr double EPSILON = 1e-6;

    FunctionAnalyzer(const std::shared_ptr<ExpressionNode>& expr);
    std::vector<std::pair<double, double>> getDomain() const;
    std::pair<double, double> getRange() const;
    bool isOdd() const;
    bool isEven() const;
    bool isSymmetric() const;
    std::vector<std::pair<double, double>> getXAxisIntercepts() const;
    std::pair<double, double> getYAxisIntercept() const;
    void plotNcurses(const std::string& filename) const;

private:
    std::shared_ptr<ExpressionNode> expr;
    std::vector<std::pair<double, double>> getRestrictedDomain(const std::shared_ptr<ExpressionNode>& node) const;
};

#endif // FUNCTIONANALYZER_H