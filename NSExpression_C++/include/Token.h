#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
enum class TokenType { NUMBER, OPERATOR, FUNCTION, PARENTHESIS};

enum class OperatorPrecedence {LOW, MEDIUM, HIGH};

class Token {
    public:
    TokenType type;
    std::string value;

    OperatorPrecedence precedence;
    Token(TokenType t, std::string v, OperatorPrecedence p = OperatorPrecedence::LOW) : type(t), value(v), precedence(p) {}

    void displayToken() const {
        std::cout << "Token Type: " << static_cast<int>(type) << ", Value: " << value << std::endl;
    }
};

#endif 