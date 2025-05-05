#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Expression.h"
#include <vector>
#include <memory>
#include <map>

class Parser {
    std::vector<Token> tokens;
    size_t position;

    Token peek() const;
    Token advance();
    Token current() const;
    bool match(const std::string& value);

    std::shared_ptr<ExpressionNode> parseExpression();
    std::shared_ptr<ExpressionNode> parseTerm();
    std::shared_ptr<ExpressionNode> parseFactor();

    public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ExpressionNode> parse();
};

#endif