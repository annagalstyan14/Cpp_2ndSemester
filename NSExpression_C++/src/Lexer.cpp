#include "Lexer.h"
#include <cctype>
#include <iostream>

char Lexer::currentChar() const {
    if (position < input.length()) {
        return input[position];
    }
    return '\0';
}

char Lexer::nextChar() {
    if (position + 1 < input.length()) {
        return input[position + 1];
    }
    return '\0';
}

void Lexer::advance() {
    if(position < input.length()){
        ++position;
    }
}

bool Lexer::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isOperator(char c) const {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool Lexer::isParathesis(char c) const {
    return c == '(' || c == ')';
}

bool Lexer::isFunction(const std::string& str) const {
    return str == "sin" || str == "cos" || str == "tan" || str == "log" || str == "sqrt";
}

void Lexer::handleNumber() {
    std::string number = "";
    while(isDigit(currentChar())) {
        number += currentChar();
        advance();
    }
    tokens.push_back(Token(TokenType::NUMBER, number));
}

void Lexer::handleOperator() {
    std::string op(1, currentChar());
    OperatorPrecedence precedence = OperatorPrecedence::LOW;
    
    if (op == "^") {
        precedence = OperatorPrecedence::HIGH;
    } else if (op == "*" || op == "/") {
        precedence = OperatorPrecedence::HIGH;
    } else if (op == "+" || op == "-") {
        precedence = OperatorPrecedence::MEDIUM;
    }
    
    tokens.push_back(Token(TokenType::OPERATOR, op, precedence));
    advance();
}

void Lexer::handleParanthesis() {
    std::string paren(1, currentChar());
    tokens.push_back(Token(TokenType::PARENTHESIS, paren));
    advance();
}

void Lexer::handleFunction() {
    std::string func = "";
    while(isalpha(currentChar())) {
        func += currentChar();
        advance();
    }
    if (isFunction(func)) {
        tokens.push_back(Token(TokenType::FUNCTION, func));
    }
}

std::vector<Token> Lexer::tokenize() {
    tokens.clear();
    while (position < input.length()) {
        char c = currentChar();
        if (isWhitespace(c)) {
            advance();
        } else if (isDigit(c)) {
            handleNumber();
        } else if (isOperator(c)) {
            handleOperator();
        } else if (isParathesis(c)) {
            handleParanthesis();
        } else if (isalpha(c)) {
            handleFunction();
        } else {
            std::cerr << "Error: Unexpected character '" << c << "'\n";
            advance();
        }
    }

    return tokens;
}