#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Expression.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

class Parser {
    std::vector<Token> tokens;
    size_t position;
    std::unordered_map<std::string, double> variables; // Add this line to store constants/variables

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
    std::shared_ptr<ExpressionNode> parseExponentiation();
    std::shared_ptr<ExpressionNode> parseUnary();
    std::shared_ptr<ExpressionNode> parsePrimary();

public:
    // Modify constructor to accept the 'variables' map
    Parser(const std::vector<Token>& tokens, const std::unordered_map<std::string, double>& variables);

    std::shared_ptr<ExpressionNode> parse();
};

#endif
