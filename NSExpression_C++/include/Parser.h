#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Expression.h"
#include <vector>
#include <memory>
#include <map>
#include <string>

class Parser {
    std::vector<Token> tokens;
    size_t position;

    Token peek() const;
    Token advance();
    Token current() const;
    bool match(const std::string& value);
    bool matchType(TokenType type);
    bool check(TokenType type) const;

    std::shared_ptr<ExpressionNode> parseExpression();
    std::shared_ptr<ExpressionNode> parseEquality();
    std::shared_ptr<ExpressionNode> parseAdditive();
    std::shared_ptr<ExpressionNode> parseMultiplicative();
    std::shared_ptr<ExpressionNode> parseExponentiation(); // <-- added this line
    std::shared_ptr<ExpressionNode> parseUnary();
    std::shared_ptr<ExpressionNode> parsePrimary();

public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ExpressionNode> parse();
};

#endif

