#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <cctype>
#include "Token.h"

class Lexer {
    public:
    Lexer(const std::string& input) : input(input), position(0) {}

    std::vector<Token> tokenize();

    private:
    std::string input;
    size_t position;
    std::vector<Token> tokens;

    char currentChar() const;
    char nextChar();
    void advance();
    bool isWhitespace(char c) const;
    bool isDigit(char c) const;
    bool isOperator(char c) const;
    bool isParathesis(char c) const;
    bool isFunction(const std::string& str) const;
    void handleNumber();
    void handleOperator();
    void handleParanthesis();
    void handleComma();
    void handleFunction();
};

#endif