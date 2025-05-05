#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>
#include <string>
#include <unordered_map>

class ExpressionNode {
public:
    virtual ~ExpressionNode() = default;
    // Updated evaluate function to accept variables map
    virtual double evaluate(const std::unordered_map<std::string, double>& variables) const = 0;
};

class BinaryOpNode : public ExpressionNode {
private:
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;
    std::string op;
    
public:
    BinaryOpNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right, const std::string& op)
        : left(left), right(right), op(op) {}
    
    double evaluate(const std::unordered_map<std::string, double>& variables) const override {
        if (op == "+") {
            return left->evaluate(variables) + right->evaluate(variables);
        } else if (op == "-") {
            return left->evaluate(variables) - right->evaluate(variables);
        } else if (op == "*") {
            return left->evaluate(variables) * right->evaluate(variables);
        } else if (op == "/") {
            return left->evaluate(variables) / right->evaluate(variables);
        }
        return 0.0;
    }
};

class UnaryOpNode : public ExpressionNode {
private:
    std::shared_ptr<ExpressionNode> operand;
    std::string op;
    
public:
    UnaryOpNode(std::shared_ptr<ExpressionNode> operand, const std::string& op)
        : operand(operand), op(op) {}
    
    double evaluate(const std::unordered_map<std::string, double>& variables) const override {
        if (op == "-") {
            return -operand->evaluate(variables);
        }
        return operand->evaluate(variables);
    }
};

#endif // EXPRESSION_H
