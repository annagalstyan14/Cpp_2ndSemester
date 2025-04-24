#include "Parser.h"
#include <cmath>
#include <stdexcept>

// Add a global symbol table
static SymbolTable symbolTable;

double BinaryOpNode::evaluate() const {
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

double UnaryOperatorNode::evaluate() const {
    double val = operand->evaluate();
    if (op == "-") return -val;
    throw std::runtime_error("Unknown unary operator: " + op);
}

double FunctionNode::evaluate() const {
    double arg = argument->evaluate();
    
    if (funcName == "sin") return std::sin(arg);
    if (funcName == "cos") return std::cos(arg);
    if (funcName == "tan") return std::tan(arg);
    if (funcName == "log") {
        if (arg <= 0) throw std::runtime_error("Logarithm of non-positive number");
        return std::log(arg);
    }
    if (funcName == "sqrt") {
        if (arg < 0) throw std::runtime_error("Square root of negative number");
        return std::sqrt(arg);
    }
    throw std::runtime_error("Unknown function: " + funcName);
}

// Add a new node type for variables and constants
class VariableNode : public ExpressionNode {
    std::string name;
public:
    VariableNode(const std::string& n) : name(n) {}
    double evaluate() const override {
        return symbolTable.getVariable(name);
    }
};
