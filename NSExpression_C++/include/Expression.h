#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <cmath>
#include <vector>
#include <map>

// ===== ABSTRACT BASE NODE =====
class ExpressionNode {
public:
    virtual double evaluate(const std::map<std::string, double>& variables = {}) const = 0;
    virtual ~ExpressionNode() = default;
};

// ===== NUMBER NODE =====
class NumberNode : public ExpressionNode {
    double value;
public:
    NumberNode(double val);
    double evaluate(const std::map<std::string, double>& variables = {}) const override;
};

// ===== BINARY OPERATOR NODE =====
class BinaryOpNode : public ExpressionNode {
    std::string op;
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;
public:
    BinaryOpNode(const std::string& oper,
                 std::shared_ptr<ExpressionNode> lhs,
                 std::shared_ptr<ExpressionNode> rhs);
    double evaluate(const std::map<std::string, double>& variables = {}) const override;
};

// ===== UNARY OPERATOR NODE =====
class UnaryOperatorNode : public ExpressionNode {
    std::string op;
    std::shared_ptr<ExpressionNode> operand;
public:
    UnaryOperatorNode(const std::string& oper,
                      std::shared_ptr<ExpressionNode> opnd);
    double evaluate(const std::map<std::string, double>& variables = {}) const override;
};

// ===== FUNCTION NODE =====
class FunctionNode : public ExpressionNode {
    std::string funcName;
    std::vector<std::shared_ptr<ExpressionNode>> arguments;
public:
    FunctionNode(const std::string& name,
                 const std::vector<std::shared_ptr<ExpressionNode>>& args);
    double evaluate(const std::map<std::string, double>& variables = {}) const override;
};

#endif // EXPRESSION_H

