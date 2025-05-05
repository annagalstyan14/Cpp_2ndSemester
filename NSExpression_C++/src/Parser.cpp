#include "../include/Parser.h"
#include "../include/Token.h"
#include "../include/Lexer.h"
#include "../include/Utilities.h"
#include <stdexcept>
#include <memory>
#include <iostream>
#include <cmath>

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

std::shared_ptr<ExpressionNode> Parser::parseFactor() {
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
        args.push_back(parseExpression());
        
        // Handle functions with two arguments (pow, root)
        if ((token.value == "pow" || token.value == "root") && match(",")) {
            args.push_back(parseExpression());
        }
        
        if (!match(")")) {
            throw std::runtime_error("Expected ')' after function arguments");
        }
        return std::make_shared<FunctionNode>(token.value, args);
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
        return std::make_shared<UnaryOperatorNode>("-", right);
    }

    throw std::runtime_error("Unexpected token in factor: " + token.value);
}

std::shared_ptr<ExpressionNode> Parser::parseTerm() {
    auto left = parseFactor();

    while (current().type != TokenType::EOF_TOKEN && (current().value == "*" || current().value == "/")) {
        std::string op = advance().value;
        auto right = parseFactor();
        left = std::make_shared<BinaryOpNode>(op, left, right);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseExpression() {
    auto left = parseTerm();

    while (current().type != TokenType::EOF_TOKEN && (current().value == "+" || current().value == "-")) {
        std::string op = advance().value;
        auto right = parseTerm();
        left = std::make_shared<BinaryOpNode>(op, left, right);
    }

    return left;
}

std::shared_ptr<ExpressionNode> Parser::parse() {
    auto expr = parseExpression();

    if (position < tokens.size() - 1) { // -1 to account for EOF token
        throw std::runtime_error("Unexpected token after end of expression: " + current().value);
    }

    return expr;
}
