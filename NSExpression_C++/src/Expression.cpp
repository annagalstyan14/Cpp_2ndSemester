#include "Parser.h"
#include "Expression.h"
#include <memory>
#include <string>
#include <iostream>

std::shared_ptr<ExpressionNode> Parser::parseExpression() {
    // For simplicity, let's assume we're parsing binary operations like "3 + 4"
    auto left = parseTerm();  // Parse left-hand side expression
    while (currentToken == "+" || currentToken == "-") {
        std::string op = currentToken;
        nextToken(); // Move to the next token
        auto right = parseTerm(); // Parse right-hand side expression
        left = std::make_shared<BinaryOpNode>(left, right, op);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseTerm() {
    // Parse multiplication or division terms here (e.g., "3 * 4")
    auto left = parseFactor();  // Assume parseFactor() is defined for parsing numbers/variables
    while (currentToken == "*" || currentToken == "/") {
        std::string op = currentToken;
        nextToken();
        auto right = parseFactor();
        left = std::make_shared<BinaryOpNode>(left, right, op);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseFactor() {
    // Implement parsing for factors (e.g., numbers, parentheses, etc.)
    // This is a simplified example
    if (currentToken == "(") {
        nextToken();
        auto expr = parseExpression();
        if (currentToken == ")") {
            nextToken();
        }
        return expr;
    } else {
        // Assume the token is a number or variable
        return std::make_shared<UnaryOpNode>(parseFactor(), "-"); // Example unary minus
    }
}
