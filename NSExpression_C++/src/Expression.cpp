#include "Expression.h"
#include <cmath>
#include <stdexcept>

// ===== SYMBOL TABLE IMPLEMENTATION =====
void SymbolTable::set(const std::string& name, double value){
    table[name] = value;
}

std::optional<double> SymbolTable::get(const std::string& name) const {
    auto it = table.find(name);
    if (it != table.end()){
        return it->second;
    }
    return std::nullopt;
}

bool SymbolTable::contains(const std::string& name) const {
    return table.find(name) != table.end();
}

void SymbolTable::clear(){
    table.clear();
}

// ===== NODE EVALUATION FUNCTIONS =====
double BinaryOpNode::evaluate(const SymbolTable& symbols) const {
    double leftVal = left->evaluate(symbols);
    double rightVal = right->evaluate(symbols);

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

double UnaryOperatorNode::evaluate(const SymbolTable& symbols) const {
    double val = operand->evaluate(symbols);
    if (op == "-") return -val;
    throw std::runtime_error("Unknown unary operator: " + op);
}

double FunctionNode::evaluate(const SymbolTable& symbols) const {
    double arg = argument->evaluate(symbols);

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

double VariableNode::evaluate(const SymbolTable& symbols) const {
    auto value = symbols.get(name);
    if (!value.has_value()) {
        throw std::runtime_error("Undefined variable: " + name);
    }
    return *value;
}
