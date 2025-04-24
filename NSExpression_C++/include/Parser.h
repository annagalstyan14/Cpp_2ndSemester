#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include <vector>
#include <memory>

class ExpressionMode {
    public:
    virtual ~ExpressionMode() = default;
    virtual double evaluate() const = 0;
};

class NumberNode: public ExpressionMode {
    double value;
    public:
    NumberNode(double val) : value(val) {}
    double evaluate() const override {return value; }
};

class BinaryOpNode : public ExpressionMode {
    char op;
    std::unique_ptr<ExpressionMode> left, right;
    public:
    BinaryOpNode(char o, std::unique_ptr<ExpressionMode> l, std::unique_ptr<ExpressionMode> r) : op(o), left(std::move(l)), right(std::move(r)) {}
    double evaluate() const override;
};

class Parser {
    std::vector<Token> tokens;
    size_t current;

    Token peek() const;
    Token advance();
    bool match(TokenType type, const std::string& value = "");

    std::unique_ptr<ExpressionMode> parseExpression();
    std::unique_ptr<ExpressionMode> parseTerm();
    std::unique_ptr<ExpressionMode> parseFactor();

    public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ExpressionMode> parse();
};

#endif