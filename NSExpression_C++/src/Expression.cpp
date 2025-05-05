#include "Expression.h"
#include <stdexcept>
#include <cmath>

// ===== ABSTRACT BASE NODE =====
double ExpressionNode::evaluate(const std::map<std::string, double>& variables) const {
    // Abstract method, should not be called directly on base class
    throw std::runtime_error("Abstract method 'evaluate' called");
}

// ===== NUMBER NODE =====
double NumberNode::evaluate(const std::map<std::string, double>& variables) const {
    return value;
}

// ===== BINARY OPERATOR NODE =====
double BinaryOpNode::evaluate(const std::map<std::string, double>& variables) const {
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

// ===== UNARY OPERATOR NODE =====
double UnaryOperatorNode::evaluate(const std::map<std::string, double>& variables) const {
    double val = operand->evaluate(variables);
    if (op == "-") return -val;
    
    throw std::runtime_error("Unknown unary operator: " + op);
}

// ===== FUNCTION NODE =====
double FunctionNode::evaluate(const std::map<std::string, double>& variables) const {
    if (arguments.empty()) {
        throw std::runtime_error("Function " + funcName + " called with no arguments");
    }

    // Single argument functions
    if (arguments.size() == 1) {
        double arg = arguments[0]->evaluate(variables);

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
        double arg1 = arguments[0]->evaluate(variables);
        double arg2 = arguments[1]->evaluate(variables);

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

