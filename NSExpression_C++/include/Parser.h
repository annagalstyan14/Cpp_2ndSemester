#ifndef PARSER_H
#define PARSER_H

#include "Expression.h"
#include "Token.h"
#include <vector>
#include <unordered_map>

class Parser {
private:
    const std::vector<Token>& tokens;
    size_t pos;
    std::unordered_map<std::string, double>& variables;

    Token currentToken() const;
    void advance();
    std::shared_ptr<ExpressionNode> parseExpression();
    std::shared_ptr<ExpressionNode> parseTerm();
    std::shared_ptr<ExpressionNode> parseFactor();
    std::shared_ptr<ExpressionNode> parsePrimary();

public:
    Parser(const std::vector<Token>& tokens, std::unordered_map<std::string, double>& variables);
    std::shared_ptr<ExpressionNode> parse();
};

#endif