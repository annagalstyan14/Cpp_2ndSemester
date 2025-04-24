#include "Parser.h"
#include "Token.h"
#include "Lexer.h"
#include "Utilities.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}

Token Parser::peek() const {
    return (position < tokens.size()) ? tokens[position] : Token(TokenType::OPERATOR, "");
}

Token Parser::advance() {
    return (position < tokens.size()) ? tokens[position++] : Token(TokenType::OPERATOR, "");
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

std::unique_ptr<ExpressionNode> Parser::parseFactor() {
    Token token = advance();

    if (token.type == TokenType::NUMBER) {
        return std::make_unique<NumberNode>(std::stod(token.value));
    }

    if (token.type == TokenType::FUNCTION) {
        if (!match("(")) {
            throw std::runtime_error("Expected '(' after function name");
        }
        auto arg = parseExpression();
        if (!match(")")) {
            throw std::runtime_error("Expected ')' after the function argument");
        }
        return std::make_unique<FunctionNode>(token.value, std::move(arg));
    }

    if (token.value == "(") {
        auto expr = parseExpression();
        if (!match(")")) {
            throw std::runtime_error("Expected ')' after expression");
        }
        return expr;
    }

    if (token.value == "-") {
        auto right = parseFactor(); // unary minus
        return std::make_unique<UnaryOperatorNode>("-", std::move(right));
    }

    throw std::runtime_error("Unexpected token in factor: " + token.value);
}

std::unique_ptr<ExpressionNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (current().value == "*" || current().value == "/") {
        std::string op = advance().value;
        auto right = parseFactor();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ExpressionNode> Parser::parseExpression() {
    auto left = parseTerm();

    while (current().value == "+" || current().value == "-") {
        std::string op = advance().value;
        auto right = parseTerm();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<ExpressionNode> Parser::parse() {
    auto expr = parseExpression();

    if (position < tokens.size()) {
        throw std::runtime_error("Unexpected token after end of expression: " + current().value);
    }

    return expr;
}
