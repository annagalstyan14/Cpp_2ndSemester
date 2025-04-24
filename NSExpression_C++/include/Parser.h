#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include <vector>
#include <memory>

class ExpressionNode {
    public:
    virtual ~ExpressionNode() = default;
    virtual double evaluate() const = 0;
};

class NumberNode: public ExpressionNode {
    double value;
    public:
    NumberNode(double val) : value(val) {}
    double evaluate() const override {return value; }
};

class BinaryOpNode : public ExpressionNode {
    std::string op;
    std::unique_ptr<ExpressionNode> left, right;
    public:
    BinaryOpNode(const std::string& o, std::unique_ptr<ExpressionNode> l, std::unique_ptr<ExpressionNode> r) 
        : op(o), left(std::move(l)), right(std::move(r)) {}
    double evaluate() const override;
};

class UnaryOperatorNode : public ExpressionNode {
    std::string op;
    std::unique_ptr<ExpressionNode> operand;
    public:
    UnaryOperatorNode(const std::string& o, std::unique_ptr<ExpressionNode> op) 
        : op(o), operand(std::move(op)) {}
    double evaluate() const override;
};

class FunctionNode : public ExpressionNode {
    std::string funcName;
    std::unique_ptr<ExpressionNode> argument;
    public:
    FunctionNode(const std::string& name, std::unique_ptr<ExpressionNode> arg) 
        : funcName(name), argument(std::move(arg)) {}
    double evaluate() const override;
};

class Parser {
    std::vector<Token> tokens;
    size_t position;

    Token peek() const;
    Token advance();
    Token current() const;
    bool match(const std::string& value);

    std::unique_ptr<ExpressionNode> parseExpression();
    std::unique_ptr<ExpressionNode> parseTerm();
    std::unique_ptr<ExpressionNode> parseFactor();

    public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ExpressionNode> parse();
};

#endif