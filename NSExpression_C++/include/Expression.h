#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>
#include <string>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
#include <vector>

class ExpressionNode {
public:
    virtual ~ExpressionNode() = default;
    virtual double evaluate(const std::unordered_map<std::string, double>& variables) = 0;
    virtual bool isEquation() const { return false; }
};

class NumberNode : public ExpressionNode {
private:
    double value;
public:
    NumberNode(double value) : value(value) {}
    double evaluate(const std::unordered_map<std::string, double>&) override {
        return value;
    }
    double getValue() const { return value; }
};

class VariableNode : public ExpressionNode {
private:
    std::string name;
public:
    VariableNode(const std::string& name) : name(name) {}
    double evaluate(const std::unordered_map<std::string, double>& variables) override {
        auto it = variables.find(name);
        if (it == variables.end()) {
            throw std::runtime_error("Undefined variable: " + name);
        }
        return it->second;
    }
    std::string getName() const { return name; }
};

class BinaryOpNode : public ExpressionNode {
private:
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;
    std::string op;
public:
    BinaryOpNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right, const std::string& op)
        : left(left), right(right), op(op) {}
    double evaluate(const std::unordered_map<std::string, double>& variables) override {
        double leftVal = left->evaluate(variables);
        double rightVal = right->evaluate(variables);
        if (op == "+") return leftVal + rightVal;
        if (op == "-") return leftVal - rightVal;
        if (op == "*") return leftVal * rightVal;
        if (op == "/") {
            if (rightVal == 0) throw std::runtime_error("Division by zero");
            return leftVal / rightVal;
        }
        if (op == "^") return std::pow(leftVal, rightVal);
        throw std::runtime_error("Unknown operator: " + op);
    }
    std::string getOp() const { return op; }
    std::shared_ptr<ExpressionNode> getLeft() const { return left; }
    std::shared_ptr<ExpressionNode> getRight() const { return right; }
};

class UnaryOpNode : public ExpressionNode {
private:
    std::shared_ptr<ExpressionNode> operand;
    std::string op;
public:
    UnaryOpNode(std::shared_ptr<ExpressionNode> operand, const std::string& op)
        : operand(operand), op(op) {}
    double evaluate(const std::unordered_map<std::string, double>& variables) override {
        double val = operand->evaluate(variables);
        if (op == "-") return -val;
        throw std::runtime_error("Unknown unary operator: " + op);
    }
    std::string getOp() const { return op; }
    std::shared_ptr<ExpressionNode> getOperand() const { return operand; }
};

class FunctionNode : public ExpressionNode {
private:
    std::string name;
    std::vector<std::shared_ptr<ExpressionNode>> args;
public:
    FunctionNode(const std::string& name, const std::vector<std::shared_ptr<ExpressionNode>>& args)
        : name(name), args(args) {}
    double evaluate(const std::unordered_map<std::string, double>& variables) override {
        if (args.size() != 1) throw std::runtime_error("Only single-argument functions supported");
        double arg = args[0]->evaluate(variables);
        if (name == "sin") return std::sin(arg);
        if (name == "cos") return std::cos(arg);
        if (name == "tan") return std::tan(arg);
        if (name == "asin") return std::asin(arg);
        if (name == "acos") return std::acos(arg);
        if (name == "atan") return std::atan(arg);
        if (name == "log" || name == "ln") {
            if (arg <= 0) throw std::runtime_error("Logarithm undefined for non-positive argument");
            return std::log(arg);
        }
        if (name == "log10") {
            if (arg <= 0) throw std::runtime_error("Log10 undefined for non-positive argument");
            return std::log10(arg);
        }
        if (name == "log2") {
            if (arg <= 0) throw std::runtime_error("Log2 undefined for non-positive argument");
            return std::log2(arg);
        }
        if (name == "exp") return std::exp(arg);
        if (name == "sqrt") {
            if (arg < 0) throw std::runtime_error("Square root undefined for negative argument");
            return std::sqrt(arg);
        }
        if (name == "cbrt") return std::cbrt(arg);
        throw std::runtime_error("Unknown function: " + name);
    }
    std::string getName() const { return name; }
    const std::vector<std::shared_ptr<ExpressionNode>>& getArgs() const { return args; }
};

class EquationNode : public ExpressionNode {
private:
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;

    // Helper function to extract coefficient and constant for linear terms
    static void extractLinear(const std::shared_ptr<ExpressionNode>& node, double& coeff, double& constant, const std::string& var) {
        if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(node)) {
            if (bin->getOp() == "+") {
                extractLinear(bin->getLeft(), coeff, constant, var);
                extractLinear(bin->getRight(), coeff, constant, var);
            } else if (bin->getOp() == "-") {
                extractLinear(bin->getLeft(), coeff, constant, var);
                double sub_coeff = 0, sub_const = 0;
                extractLinear(bin->getRight(), sub_coeff, sub_const, var);
                coeff -= sub_coeff;
                constant -= sub_const;
            } else if (bin->getOp() == "*") {
                if (auto varNode = std::dynamic_pointer_cast<VariableNode>(bin->getLeft())) {
                    if (varNode->getName() == var) {
                        if (auto numNode = std::dynamic_pointer_cast<NumberNode>(bin->getRight())) {
                            coeff += numNode->getValue();
                        }
                    }
                } else if (auto varNode = std::dynamic_pointer_cast<VariableNode>(bin->getRight())) {
                    if (varNode->getName() == var) {
                        if (auto numNode = std::dynamic_pointer_cast<NumberNode>(bin->getLeft())) {
                            coeff += numNode->getValue();
                        }
                    }
                }
            }
        } else if (auto varNode = std::dynamic_pointer_cast<VariableNode>(node)) {
            if (varNode->getName() == var) {
                coeff += 1.0;
            }
        } else if (auto numNode = std::dynamic_pointer_cast<NumberNode>(node)) {
            constant += numNode->getValue();
        } else if (auto unary = std::dynamic_pointer_cast<UnaryOpNode>(node)) {
            if (unary->getOp() == "-") {
                double sub_coeff = 0, sub_const = 0;
                extractLinear(unary->getOperand(), sub_coeff, sub_const, var);
                coeff -= sub_coeff;
                constant -= sub_const;
            }
        }
    }

public:
    EquationNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right)
        : left(left), right(right) {}
    double evaluate(const std::unordered_map<std::string, double>& variables) override {
        return left->evaluate(variables) - right->evaluate(variables);
    }
    bool isEquation() const override { return true; }

    // Solve for a variable (e.g., "x") in a linear equation ax + b = c
    double solveFor(const std::string& var, const std::unordered_map<std::string, double>& variables) const {
        // Simplify: Move all terms to one side (left - right = 0)
        auto simplified = std::make_shared<BinaryOpNode>(left, right, "-");

        double coeff = 0, constant = 0;
        extractLinear(simplified, coeff, constant, var);

        if (std::abs(coeff) < 1e-10) {
            if (std::abs(constant) < 1e-10) {
                throw std::runtime_error("Equation is an identity (infinite solutions)");
            } else {
                throw std::runtime_error("No solution (inconsistent equation)");
            }
        }

        return -constant / coeff; // Solution: x = -constant / coeff
    }
};

#endif