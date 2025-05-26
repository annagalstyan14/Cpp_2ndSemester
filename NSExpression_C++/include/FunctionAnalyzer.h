#ifndef FUNCTION_ANALYZER_H
#define FUNCTION_ANALYZER_H

#include <memory>
#include <vector>
#include <utility>
#include <string>

class ExpressionNode;
class FunctionNode;
class BinaryOpNode;
class UnaryOpNode;
class VariableNode;
class NumberNode;

class FunctionAnalyzer {
public:
    explicit FunctionAnalyzer(const std::shared_ptr<ExpressionNode>& expr);
    
    // Get the domain of the function
    std::vector<std::pair<double, double>> getDomain() const;
    
    // Get the range of the function
    std::pair<double, double> getRange() const;
    
    // Check if the function is even
    bool isEven() const;
    
    // Check if the function is odd
    bool isOdd() const;
    
    // Check if the function is symmetric
    bool isSymmetric() const;
    
    // Get x-axis intercepts
    std::vector<std::pair<double, double>> getXAxisIntercepts() const;
    
    // Get y-axis intercept
    std::pair<double, double> getYAxisIntercept() const;
    
    // Plot the function using ncurses
    void plotNcurses(const std::string& filename = "") const;

private:
    std::shared_ptr<ExpressionNode> expr;
    
    // Helper method to get restricted domain
    std::vector<std::pair<double, double>> getRestrictedDomain(const std::shared_ptr<ExpressionNode>& node) const;
    
    // Constants for plotting
    static constexpr double PLOT_MIN = -10.0;
    static constexpr double PLOT_MAX = 10.0;
    static constexpr double PLOT_STEP = 0.1;
    static constexpr double EPSILON = 1e-10;
};

#endif // FUNCTION_ANALYZER_H