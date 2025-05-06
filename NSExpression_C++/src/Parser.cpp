#include "Parser.h"
#include <stdexcept>
#include <cmath>

Parser::Parser(const std::vector<Token>& tokens, const std::unordered_map<std::string, double>& variables)
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
    if (position < tokens.size() && peek().type != TokenType::EOF_TOKEN) {
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
        left = std::make_shared<BinaryOpNode>(left, right, op);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseMultiplicative() {
    auto left = parseExponentiation();
    while (current().value == "*" || current().value == "/") {
        std::string op = advance().value;
        auto right = parseExponentiation();
        left = std::make_shared<BinaryOpNode>(left, right, op);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseExponentiation() {
    auto left = parseUnary();
    while (current().value == "^") {
        std::string op = advance().value;
        auto right = parseUnary();
        left = std::make_shared<BinaryOpNode>(left, right, op);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseUnary() {
    if (match("-")) {
        auto operand = parseUnary();
        return std::make_shared<UnaryOpNode>(operand, "-");
    }
    return parsePrimary();
}

std::shared_ptr<ExpressionNode> Parser::parsePrimary() {
    if (matchType(TokenType::NUMBER)) {
        return std::make_shared<NumberNode>(std::stod(tokens[position - 1].value));
    }
    if (matchType(TokenType::IDENTIFIER) || matchType(TokenType::FUNCTION)) {
        std::string name = tokens[position - 1].value;
        if (match("(")) {
            auto arg = parse();
            if (!match(")")) {
                throw std::runtime_error("Expected ')' after function argument");
            }
            return std::make_shared<FunctionNode>(name, std::vector<std::shared_ptr<ExpressionNode>>{arg});
        }
        return std::make_shared<VariableNode>(name);
    }
    if (matchType(TokenType::CONSTANT)) {
        return std::make_shared<VariableNode>(tokens[position - 1].value);
    }
    if (match("(")) {
        auto expr = parse();
        if (!match(")")) {
            throw std::runtime_error("Expected ')' after expression");
        }
        return expr;
    }
    throw std::runtime_error("Unexpected token: " + current().value);
}