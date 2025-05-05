#include "../include/Parser.h"
#include "../include/Token.h"
#include "../include/Expression.h"
#include <stdexcept>
#include <cmath>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}

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
        auto right = parseEquality(); // recursion to allow chained = (e.g., a = b = 2)
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
        advance(); // skip '^'
        auto right = parseUnary();  // right-associative (could use parseExponentiation here if you want true right-associativity)
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
    Token token = advance();

    if (token.type == TokenType::NUMBER) {
        return std::make_shared<NumberNode>(std::stod(token.value));
    } else if (token.type == TokenType::VARIABLE) {
        return std::make_shared<VariableNode>(token.value);
    } else if (token.type == TokenType::FUNCTION) {
        if (!match("(")) {
            throw std::runtime_error("Expected '(' after function name");
        }

        std::vector<std::shared_ptr<ExpressionNode>> args;
        args.push_back(parse());

        if ((token.value == "pow" || token.value == "root") && match(",")) {
            args.push_back(parse());
        }

        if (!match(")")) {
            throw std::runtime_error("Expected ')' after function arguments");
        }

        return std::make_shared<FunctionNode>(token.value, args);
    } else if (token.value == "(") {
        auto expr = parse();
        if (!match(")")) {
            throw std::runtime_error("Expected ')' after expression");
        }
        return expr;
    }

    throw std::runtime_error("Unexpected token in primary: " + token.value);
}

