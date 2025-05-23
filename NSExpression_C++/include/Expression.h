#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>
#include <string>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
#include <vector>
#include "Complex.h"

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

    // Helper function to evaluate expression with complex numbers
    Complex evaluateComplex(const std::shared_ptr<ExpressionNode>& node, const Complex& x) const {
        if (auto num = std::dynamic_pointer_cast<NumberNode>(node)) {
            return Complex(num->getValue());
        } else if (auto var = std::dynamic_pointer_cast<VariableNode>(node)) {
            return x;
        } else if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(node)) {
            Complex left = evaluateComplex(bin->getLeft(), x);
            Complex right = evaluateComplex(bin->getRight(), x);
            if (bin->getOp() == "+") return left + right;
            if (bin->getOp() == "-") return left - right;
            if (bin->getOp() == "*") return left * right;
            if (bin->getOp() == "/") return left / right;
            if (bin->getOp() == "^") {
                if (std::abs(right.getImag()) < 1e-10) {
                    double power = right.getReal();
                    if (power == 2) return left * left;
                    if (power == 3) return left * left * left;
                    // For other powers, we'll need to implement complex power
                    throw std::runtime_error("Complex power not implemented");
                }
                throw std::runtime_error("Complex power not implemented");
            }
            throw std::runtime_error("Unknown operator: " + bin->getOp());
        } else if (auto unary = std::dynamic_pointer_cast<UnaryOpNode>(node)) {
            Complex operand = evaluateComplex(unary->getOperand(), x);
            if (unary->getOp() == "-") return Complex(-operand.getReal(), -operand.getImag());
            throw std::runtime_error("Unknown unary operator: " + unary->getOp());
        } else if (auto func = std::dynamic_pointer_cast<FunctionNode>(node)) {
            Complex arg = evaluateComplex(func->getArgs()[0], x);
            // For now, we'll only handle basic functions with real arguments
            if (std::abs(arg.getImag()) > 1e-10) {
                throw std::runtime_error("Complex function evaluation not implemented");
            }
            double realArg = arg.getReal();
            if (func->getName() == "sin") return Complex(std::sin(realArg));
            if (func->getName() == "cos") return Complex(std::cos(realArg));
            if (func->getName() == "tan") return Complex(std::tan(realArg));
            if (func->getName() == "exp") return Complex(std::exp(realArg));
            if (func->getName() == "sqrt") {
                if (realArg < 0) {
                    return Complex(0, std::sqrt(-realArg));
                }
                return Complex(std::sqrt(realArg));
            }
            throw std::runtime_error("Unknown function: " + func->getName());
        }
        throw std::runtime_error("Unknown node type");
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

    // Solve for complex roots
    std::vector<Complex> solveComplex(const std::string& var) const {
        std::vector<Complex> solutions;
        
        // For quadratic equations (ax^2 + bx + c = 0)
        if (auto bin = std::dynamic_pointer_cast<BinaryOpNode>(left)) {
            if (bin->getOp() == "+") {
                if (auto leftBin = std::dynamic_pointer_cast<BinaryOpNode>(bin->getLeft())) {
                    if (leftBin->getOp() == "*") {
                        // Check if it's a quadratic term (ax^2)
                        if (auto rightBin = std::dynamic_pointer_cast<BinaryOpNode>(leftBin->getRight())) {
                            if (rightBin->getOp() == "^") {
                                if (auto varNode = std::dynamic_pointer_cast<VariableNode>(rightBin->getLeft())) {
                                    if (varNode->getName() == var) {
                                        if (auto powerNode = std::dynamic_pointer_cast<NumberNode>(rightBin->getRight())) {
                                            if (std::abs(powerNode->getValue() - 2.0) < 1e-10) {
                                                // We have a quadratic equation
                                                double a = 0, b = 0, c = 0;
                                                
                                                // Extract coefficients
                                                if (auto aNode = std::dynamic_pointer_cast<NumberNode>(leftBin->getLeft())) {
                                                    a = aNode->getValue();
                                                }
                                                
                                                // Extract linear term (bx)
                                                if (auto rightTerm = std::dynamic_pointer_cast<BinaryOpNode>(bin->getRight())) {
                                                    if (rightTerm->getOp() == "*") {
                                                        if (auto bNode = std::dynamic_pointer_cast<NumberNode>(rightTerm->getLeft())) {
                                                            if (auto varNode = std::dynamic_pointer_cast<VariableNode>(rightTerm->getRight())) {
                                                                if (varNode->getName() == var) {
                                                                    b = bNode->getValue();
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                
                                                // Extract constant term (c)
                                                if (auto cNode = std::dynamic_pointer_cast<NumberNode>(right)) {
                                                    c = -cNode->getValue();
                                                }
                                                
                                                // Solve quadratic equation
                                                double discriminant = b * b - 4 * a * c;
                                                if (std::abs(discriminant) < 1e-10) {
                                                    // One real root
                                                    solutions.push_back(Complex(-b / (2 * a)));
                                                } else if (discriminant > 0) {
                                                    // Two real roots
                                                    solutions.push_back(Complex((-b + std::sqrt(discriminant)) / (2 * a)));
                                                    solutions.push_back(Complex((-b - std::sqrt(discriminant)) / (2 * a)));
                                                } else {
                                                    // Two complex roots
                                                    solutions.push_back(Complex(-b / (2 * a), std::sqrt(-discriminant) / (2 * a)));
                                                    solutions.push_back(Complex(-b / (2 * a), -std::sqrt(-discriminant) / (2 * a)));
                                                }
                                                return solutions;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // For other equations, use Newton's method with complex numbers
        const double h = 1e-6;
        const int max_iterations = 100;
        const double tolerance = 1e-6;
        
        // Try initial guesses
        std::vector<Complex> initialGuesses = {
            Complex(1, 0), Complex(-1, 0), Complex(0, 1), Complex(0, -1),
            Complex(1, 1), Complex(-1, 1), Complex(1, -1), Complex(-1, -1)
        };
        
        for (const Complex& x0 : initialGuesses) {
            Complex x = x0;
            for (int i = 0; i < max_iterations; ++i) {
                Complex fx = evaluateComplex(left, x) - evaluateComplex(right, x);
                if (fx.magnitude() < tolerance) {
                    bool is_unique = true;
                    for (const Complex& sol : solutions) {
                        if ((x - sol).magnitude() < tolerance) {
                            is_unique = false;
                            break;
                        }
                    }
                    if (is_unique) {
                        solutions.push_back(x);
                    }
                    break;
                }
                
                // Complex derivative approximation
                Complex fx_plus_h = evaluateComplex(left, x + Complex(h, 0)) - evaluateComplex(right, x + Complex(h, 0));
                Complex fx_plus_hi = evaluateComplex(left, x + Complex(0, h)) - evaluateComplex(right, x + Complex(0, h));
                Complex dfx = Complex((fx_plus_h.getReal() - fx.getReal()) / h,
                                    (fx_plus_hi.getImag() - fx.getImag()) / h);
                
                if (dfx.magnitude() < 1e-10) break;
                x = x - fx / dfx;
            }
        }
        
        return solutions;
    }

    std::vector<double> solveNonLinear(const std::string& var, const std::unordered_map<std::string, double>& variables) const;
};

#endif