#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <cmath>
#include <vector>

// ===== ABSTRACT BASE NODE =====
class ExpressionNode {
public:
    virtual double evaluate() const = 0;
    virtual ~ExpressionNode() = default;
};

// ===== NUMBER NODE =====
class NumberNode : public ExpressionNode {
    double value;
public:
    NumberNode(double val) : value(val) {}
    double evaluate() const override { return value; }
};

// ===== BINARY OPERATOR NODE =====
class BinaryOpNode : public ExpressionNode {
    std::string op;
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;
public:
    BinaryOpNode(const std::string& oper,
                 std::shared_ptr<ExpressionNode> lhs,
                 std::shared_ptr<ExpressionNode> rhs)
        : op(oper), left(lhs), right(rhs) {}

    double evaluate() const override {
        double leftVal = left->evaluate();
        double rightVal = right->evaluate();

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
};

// ===== UNARY OPERATOR NODE =====
class UnaryOperatorNode : public ExpressionNode {
    std::string op;
    std::shared_ptr<ExpressionNode> operand;
public:
    UnaryOperatorNode(const std::string& oper,
                      std::shared_ptr<ExpressionNode> opnd)
        : op(oper), operand(opnd) {}

    double evaluate() const override {
        double val = operand->evaluate();
        if (op == "-") return -val;
        throw std::runtime_error("Unknown unary operator: " + op);
    }
};

// ===== FUNCTION NODE =====
class FunctionNode : public ExpressionNode {
    std::string funcName;
    std::vector<std::shared_ptr<ExpressionNode>> arguments;
public:
    FunctionNode(const std::string& name,
                 const std::vector<std::shared_ptr<ExpressionNode>>& args)
        : funcName(name), arguments(args) {}

    double evaluate() const override {
        if (arguments.empty()) {
            throw std::runtime_error("Function " + funcName + " called with no arguments");
        }

        // Single argument functions
        if (arguments.size() == 1) {
            double arg = arguments[0]->evaluate();

            // Trigonometric functions
            if (funcName == "sin") return std::sin(arg);
            if (funcName == "cos") return std::cos(arg);
            if (funcName == "tan") return std::tan(arg);
            
            // Inverse trigonometric functions
            if (funcName == "asin" || funcName == "arcsin") return std::asin(arg);
            if (funcName == "acos" || funcName == "arccos") return std::acos(arg);
            if (funcName == "atan" || funcName == "arctan") return std::atan(arg);
            
            // Logarithmic functions
            if (funcName == "log") {
                if (arg <= 0) throw std::runtime_error("Logarithm of non-positive number");
                return std::log(arg);
            }
            if (funcName == "ln") {
                if (arg <= 0) throw std::runtime_error("Natural logarithm of non-positive number");
                return std::log(arg);
            }
            if (funcName == "log10") {
                if (arg <= 0) throw std::runtime_error("Logarithm base 10 of non-positive number");
                return std::log10(arg);
            }
            if (funcName == "log2") {
                if (arg <= 0) throw std::runtime_error("Logarithm base 2 of non-positive number");
                return std::log2(arg);
            }
            
            // Exponential and root functions
            if (funcName == "exp") return std::exp(arg);
            if (funcName == "sqrt") {
                if (arg < 0) throw std::runtime_error("Square root of negative number");
                return std::sqrt(arg);
            }
            if (funcName == "cbrt") return std::cbrt(arg);
            
            // Factorial
            if (funcName == "fact" || funcName == "factorial") {
                if (arg < 0) throw std::runtime_error("Factorial of negative number");
                if (arg != std::floor(arg)) throw std::runtime_error("Factorial of non-integer number");
                double result = 1;
                for (int i = 2; i <= static_cast<int>(arg); ++i) {
                    result *= i;
                }
                return result;
            }
        }
        
        // Two argument functions
        if (arguments.size() == 2) {
            double arg1 = arguments[0]->evaluate();
            double arg2 = arguments[1]->evaluate();
            
            if (funcName == "pow") {
                if (arg1 == 0 && arg2 < 0) throw std::runtime_error("Zero to negative power");
                return std::pow(arg1, arg2);
            }
            if (funcName == "root") {
                if (arg2 == 0) throw std::runtime_error("Root of degree zero");
                if (arg1 < 0 && std::fmod(arg2, 2) == 0) throw std::runtime_error("Even root of negative number");
                return std::pow(arg1, 1.0 / arg2);
            }
        }

        throw std::runtime_error("Unknown function or invalid number of arguments: " + funcName);
    }
};

#endif // EXPRESSION_H
