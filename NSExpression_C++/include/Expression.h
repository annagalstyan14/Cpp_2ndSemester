#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>
#include <map>
#include <string>
#include <stdexcept>

class ExpressionNode {
public:
    virtual ~ExpressionNode() = default;
    virtual double evaluate(const std::map<std::string, double>& variables = {}) const = 0;
};

class NumberExpression : public ExpressionNode {
    double value;
public:
    NumberExpression(double value) : value(value) {}
    double evaluate(const std::map<std::string, double>& variables) const override {
        return value;
    }
};

class VariableExpression : public ExpressionNode {
private:
    std::string name;
public:
    VariableExpression(const std::string& name) : name(name) {}
    double evaluate(const std::map<std::string, double>& variables) const override {
        auto it = variables.find(name);
        if (it == variables.end()) {
            throw std::runtime_error("Variable " + name + " not found");
        }
        return it->second;
    }
};

class BinaryExpression : public ExpressionNode {
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;
    std::string op;
public:
    BinaryExpression(std::shared_ptr<ExpressionNode> left, const std::string& op, std::shared_ptr<ExpressionNode> right)
        : left(left), right(right), op(op) {}
    double evaluate(const std::map<std::string, double>& variables) const override {
        double leftValue = left->evaluate(variables);
        double rightValue = right->evaluate(variables);
        if (op == "+") return leftValue + rightValue;
        if (op == "-") return leftValue - rightValue;
        if (op == "*") return leftValue * rightValue;
        if (op == "/") return leftValue / rightValue;
        throw std::runtime_error("Unknown operator: " + op);
    }
};

#endif // EXPRESSION_H
