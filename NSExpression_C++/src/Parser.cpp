#include "Parser.h"
#include "Lexer.h"
#include "Token.h"
#include "Utilities.h"
#include <stdexcept>

Parser::Parser(cosnt std::vector<Token>& tokens) : tokens(tokens), position(0){}

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