#include "Parser.h"
#include "Token.h"
#include "Expression.h"
#include <stdexcept>
#include <cmath>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens, const std::map<std::string, double>& variables)
    : tokens(tokens), position(0), variables(variables) {}

Token Parser::peek() const {
    return (position < tokens.size()) ? tokens[position] : Token(TokenType::EOF_TOKEN, "");
}

Token Parser::advance() {
    return (position < tokens.size()) ? tokens[position++] : Token(TokenType::EOF_TOKEN, "");
}

Token Parser::current() const {
    return peek();
}

bool Parser::match(const std::string& value) {
    if (peek().value == value) {
        advance();
        return true;
    }
    return false;
}

bool Parser::matchType(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    return peek().type == type;
}

std::shared_ptr<ExpressionNode> Parser::parse() {
    auto expr = parseEquality();

    if (position < tokens.size() - 1) {
        throw std::runtime_error("Unexpected token after expression: " + current().value);
    }

    return expr;
}

std::shared_ptr<ExpressionNode> Parser::parseEquality() {
    auto left = parseAdditive();

    if (match("=")) {
        auto right = parseEquality();
        return std::make_shared<EquationNode>(left, right);
    }

    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseAdditive() {
    auto left = parseMultiplicative();

    while (current().value == "+" || current().value == "-") {
        std::string op = advance().value;
        auto right = parseMultiplicative();
        left = std::make_shared<BinaryOpNode>(op, left, right);
    }

    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseMultiplicative() {
    auto left = parseExponentiation();

    while (current().value == "*" || current().value == "/") {
        std::string op = advance().value;
        auto right = parseExponentiation();
        left = std::make_shared<BinaryOpNode>(op, left, right);
    }

    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseExponentiation() {
    auto left = parseUnary();

    while (current().value == "^") {
        advance();
        auto right = parseUnary();
        left = std::make_shared<BinaryOpNode>("^", left, right);
    }

    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseUnary() {
    if (match("-")) {
        auto operand = parseUnary();
        return std::make_shared<UnaryOperatorNode>("-", operand);
    }

    return parsePrimary();
}

std::shared_ptr<ExpressionNode> Parser::parsePrimary() {
    if (matchType(TokenType::NUMBER)) {
        return std::make_shared<NumberNode>(std::stod(tokens[position - 1].value));
    }

    if (matchType(TokenType::IDENTIFIER)) {
        std::string name = tokens[position - 1].value;
        if (match("(")) {
            auto arg = parse();
            if (!matchType(TokenType::PARENTHESIS) || current().value != ")") {
                throw std::runtime_error("Expected ')'");
            }
            advance(); // consume ')'
            return std::make_shared<FunctionNode>(name, std::vector<std::shared_ptr<ExpressionNode>>{arg});
        }
        return std::make_shared<VariableNode>(name);
    }
    

    if (matchType(TokenType::CONSTANT)) {
        return std::make_shared<VariableNode>(tokens[position - 1].value);
    }

    // Handle function calls like sin(), cos(), etc.
    

        if (!matchType(TokenType::PARENTHESIS) || current().value != ")") {
            throw std::runtime_error("Expected ')'");
        }

        // Create a FunctionNode with a vector of arguments
        return std::make_shared<FunctionNode>(funcName, std::vector<std::shared_ptr<ExpressionNode>>{arg});
    }

    if (match("(")) {
        auto expr = parse();
        if (!match(")")) {
            throw std::runtime_error("Expected ')'");
        }
        return expr;
    }
     {
        advance(); // consume '('
        auto expr = parse();
        if (!matchType(TokenType::PARENTHESIS) || current().value != ")") {
            throw std::runtime_error("Expected ')'");
        }
        advance(); // consume ')'
        return expr;
    }

    throw std::runtime_error("Unexpected token in primary: " + current().value);

