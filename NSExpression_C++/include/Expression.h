#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

class ExpressionNode {
    public:
    virtual ~ExpressionNode() = default;
    virtual double evaluate() const = 0;
};

class BinaryOpNode : public ExpressionNode {
    std::string op;
    ExpressionNode* left;
    ExpressionNode* right;

    public:
    BinaryOpNode(const std::string& oper, ExpressionNode* lhs, ExpressionNode*rhs) : op(oper), left(lhs), right(rhs) {}
    double evaluate() const override;
};

class UnaryOpNode : public ExpressionNode {
    std::string op;
    ExpressionNode* operand;

    public:
    UnaryOpNode(const std::string& oper, ExpressionNode* expr) :op(oper), operand(expr){}
    double evaluate() const override;
};

class NumberNode : public ExpressionNode{
    double value;

    public:
    NumberNode(double val): value(val){}
    double evaluate() const override{ return value; }
};

class FunctionNode : public ExpressionNode {
    std::string funcName;
    ExpressionNode* argument;

    public:
    FunctionNode(const std::string& name, ExpressionNode* arg) : funcName(name), argument(arg){}
    double evaluate() const override;
};

/*class SymbolTable {
    public:
    double getVariable(const std::string& name) const {
        throw std::runtime_error("Undefined variable: " + name);
    }
};*/

class VariableNode: public ExpressionNode {
    std::string name;
    public:
    VariableNode(const std::string& n) : name(n){}
    double evaluate() const override;
};

#endif