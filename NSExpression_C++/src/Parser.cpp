#include "Parser.h"
#include "Lexer.h"
#include "Token.h"
#include "Utilities.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0){}

Token Parser::peek() const {
    return (position < tokens.size()) ? tokens[position] : Token(TokenType::OPERATOR, "");
}

Token Parser::advance() {
    return (position < tokens.size()) ? tokens[position++] : Token(TokenType::OPERATOR, "");
}

bool Parser::match(const std::string& value){
    if (peek().value == value){
        advance();
        return true;
    }
    return false;
}

ExpressionNode* Parser::parseFactor() {
    Token token = advance();

    if (token.type == TokenType::NUMBER){
        return new NumberNode(std::stod(token.value));
    }

    if (token.type == TokenType::FUNCTION){
        if (!match("(")) {
            throw std::runtime_error("Expected '(' after fucntion name");
        }
        ExpressionNode* arg = parseExpression();
        if (!match("(")) {
            throw std::runtime_error("Expected ')' after the function argument");
        }
        return new FunctionNode(token.value, arg);
    }

    if (token.value == "("){
        ExpressionNode
    }
}