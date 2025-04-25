#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

enum class TokenType { 
    NUMBER, 
    OPERATOR, 
    FUNCTION, 
    PARENTHESIS,
    COMMA,
    EOF_TOKEN
};

enum class OperatorPrecedence {
    LOW, 
    MEDIUM, 
    HIGH
};

class Token {
    public:
    TokenType type;
    std::string value;
    OperatorPrecedence precedence;
    
    Token(TokenType t, std::string v) 
        : type(t), value(v), precedence(OperatorPrecedence::LOW) {}

    Token(TokenType t, std::string v, OperatorPrecedence p)
        : type(t), value(v), precedence(p) {}

    void displayToken() const {
        std::cout << "Token Type: " << static_cast<int>(type) << ", Value: " << value << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& token){
        os << "Token Type: " << static_cast<int>(token.type) << ", Value:  " << token.value;
        return os;
    }
};

#endif 