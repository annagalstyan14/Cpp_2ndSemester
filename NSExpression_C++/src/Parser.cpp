#include "Parser.h"
#include <stdexcept>
#include <cmath>

Parser::Parser(const std::vector<Token>& tokens, std::unordered_map<std::string, double>& variables)
    : tokens(tokens), pos(0), variables(variables) {}

Token Parser::currentToken() const {
    if (pos < tokens.size()) {
        return tokens[pos];
    }
    return Token{TokenType::EOF_TOKEN, ""};
}

void Parser::advance() {
    ++pos;
}

std::shared_ptr<ExpressionNode> Parser::parseExpression() {
    auto left = parseTerm();
    while (pos < tokens.size() && currentToken().type == TokenType::OPERATOR) {
        std::string op = currentToken().value;
        if (op != "+" && op != "-") break;
        advance();
        auto right = parseTerm();
        left = std::make_shared<BinaryOpNode>(left, right, op);
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseTerm() {
    auto left = parseFactor();
    while (pos < tokens.size()) {
        if (currentToken().type == TokenType::OPERATOR) {
            std::string op = currentToken().value;
            if (op != "*" && op != "/" && op != "^") break;
            advance();
            auto right = parseFactor();
            left = std::make_shared<BinaryOpNode>(left, right, op);
        } else if (currentToken().type == TokenType::VARIABLE ||
                   (currentToken().type == TokenType::PARENTHESIS && currentToken().value == "(") ||
                   currentToken().type == TokenType::IDENTIFIER ||
                   currentToken().type == TokenType::CONSTANT ||
                   currentToken().type == TokenType::FUNCTION) {
            auto right = parseFactor();
            left = std::make_shared<BinaryOpNode>(left, right, "*");
        } else {
            break;
        }
    }
    return left;
}

std::shared_ptr<ExpressionNode> Parser::parseFactor() {
    if (pos < tokens.size() && currentToken().type == TokenType::OPERATOR && currentToken().value == "-") {
        advance();
        auto operand = parseFactor();
        return std::make_shared<UnaryOpNode>(operand, "-");
    }
    return parsePrimary();
}

std::shared_ptr<ExpressionNode> Parser::parsePrimary() {
    if (pos >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }

    Token token = currentToken();
    advance();

    if (token.type == TokenType::NUMBER) {
        try {
            double value = std::stod(token.value);
            return std::make_shared<NumberNode>(value);
        } catch (const std::exception&) {
            throw std::runtime_error("Invalid number: " + token.value);
        }
    } else if (token.type == TokenType::VARIABLE || token.type == TokenType::IDENTIFIER || token.type == TokenType::CONSTANT) {
        return std::make_shared<VariableNode>(token.value);
    } else if (token.type == TokenType::FUNCTION) {
        if (pos < tokens.size() && currentToken().type == TokenType::PARENTHESIS && currentToken().value == "(") {
            advance();
            std::vector<std::shared_ptr<ExpressionNode>> args;
            if (pos < tokens.size() && !(currentToken().type == TokenType::PARENTHESIS && currentToken().value == ")")) {
                args.push_back(parseExpression());
            }
            if (pos >= tokens.size() || !(currentToken().type == TokenType::PARENTHESIS && currentToken().value == ")")) {
                throw std::runtime_error("Expected closing parenthesis in function call");
            }
            advance();
            return std::make_shared<FunctionNode>(token.value, args);
        }
        throw std::runtime_error("Expected parenthesis after function: " + token.value);
    } else if (token.type == TokenType::PARENTHESIS && token.value == "(") {
        auto expr = parseExpression();
        if (pos >= tokens.size() || !(currentToken().type == TokenType::PARENTHESIS && currentToken().value == ")")) {
            throw std::runtime_error("Expected closing parenthesis");
        }
        advance();
        return expr;
    } else {
        throw std::runtime_error("Unexpected token: " + token.value);
    }
}

std::shared_ptr<ExpressionNode> Parser::parse() {
    if (tokens.empty()) {
        throw std::runtime_error("Empty input");
    }

    auto expr = parseExpression();

    if (pos < tokens.size() && currentToken().type == TokenType::EQUALS) {
        advance();
        auto right = parseExpression();
        expr = std::make_shared<EquationNode>(expr, right);
    }

    if (pos < tokens.size() && currentToken().type != TokenType::EOF_TOKEN) {
        throw std::runtime_error("Unexpected token after expression: " + currentToken().value);
    }

    return expr;
}

NumberNode::NumberNode(double value) : value(value) {}

double NumberNode::evaluate(const std::unordered_map<std::string, double>& variables) {
    return value;
}

VariableNode::VariableNode(const std::string& name) : name(name) {}

double VariableNode::evaluate(const std::unordered_map<std::string, double>& variables) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        throw std::runtime_error("Undefined variable: " + name);
    }
    return it->second;
}

BinaryOpNode::BinaryOpNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right, const std::string& op)
    : left(left), right(right), op(op) {}

double BinaryOpNode::evaluate(const std::unordered_map<std::string, double>& variables) {
    double leftVal = left->evaluate(variables);
    double rightVal = right->evaluate(variables);
    if (op == "+") return leftVal + rightVal;
    if (op == "-") return leftVal - rightVal;
    if (op == "*") return leftVal * rightVal;
    if (op == "/") {
        if (std::abs(rightVal) < 1e-10) throw std::runtime_error("Division by zero");
        return leftVal / rightVal;
    }
    if (op == "^") return std::pow(leftVal, rightVal);
    throw std::runtime_error("Unknown operator: " + op);
}

UnaryOpNode::UnaryOpNode(std::shared_ptr<ExpressionNode> operand, const std::string& op)
    : operand(operand), op(op) {}

double UnaryOpNode::evaluate(const std::unordered_map<std::string, double>& variables) {
    double val = operand->evaluate(variables);
    if (op == "-") return -val;
    throw std::runtime_error("Unknown unary operator: " + op);
}

FunctionNode::FunctionNode(const std::string& name, const std::vector<std::shared_ptr<ExpressionNode>>& args)
    : name(name), args(args) {}

double FunctionNode::evaluate(const std::unordered_map<std::string, double>& variables) {
    if (args.empty()) {
        throw std::runtime_error("Function " + name + " requires at least one argument");
    }
    double arg = args[0]->evaluate(variables);
    if (name == "sin") return std::sin(arg);
    if (name == "cos") return std::cos(arg);
    if (name == "tan") return std::tan(arg);
    if (name == "asin" || name == "arcsin") return std::asin(arg);
    if (name == "acos" || name == "arccos") return std::acos(arg);
    if (name == "atan" || name == "arctan") return std::atan(arg);
    if (name == "log" || name == "ln") {
        if (arg <= 0) throw std::runtime_error("Logarithm of non-positive number");
        return std::log(arg);
    }
    if (name == "log10") {
        if (arg <= 0) throw std::runtime_error("Logarithm of non-positive number");
        return std::log10(arg);
    }
    if (name == "log2") {
        if (arg <= 0) throw std::runtime_error("Logarithm of non-positive number");
        return std::log2(arg);
    }
    if (name == "exp") return std::exp(arg);
    if (name == "sqrt") {
        if (arg < 0) throw std::runtime_error("Square root of negative number");
        return std::sqrt(arg);
    }
    if (name == "cbrt") return std::cbrt(arg);
    throw std::runtime_error("Unknown function: " + name);
}

EquationNode::EquationNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right)
    : left(left), right(right) {}

double EquationNode::evaluate(const std::unordered_map<std::string, double>& variables) {
    throw std::runtime_error("Cannot evaluate an equation directly");
}

double EquationNode::solveFor(const std::string& var, const std::unordered_map<std::string, double>& variables) {
    // Simplified linear solver (for backward compatibility)
    auto leftVal = std::dynamic_pointer_cast<BinaryOpNode>(left);
    auto rightVal = std::dynamic_pointer_cast<NumberNode>(right);
    if (!leftVal || !rightVal) {
        throw std::runtime_error("Only simple linear equations are supported in solveFor");
    }
    if (leftVal->evaluate(variables) != leftVal->evaluate(variables)) { // Check for NaN
        throw std::runtime_error("Invalid equation");
    }
    // Assume form ax + b = c
    double a = 0, b = 0, c = rightVal->evaluate(variables);
    // Implement linear solving logic (placeholder)
    throw std::runtime_error("Linear solving not fully implemented");
}

std::vector<double> EquationNode::solveNonLinear(const std::string& var, const std::unordered_map<std::string, double>& variables) {
    std::vector<double> solutions;
    const double h = 1e-6;
    const int max_iterations = 100;
    const double tolerance = 1e-6;

    // Define f(x) = left - right
    auto f = [&](double x) {
        std::unordered_map<std::string, double> vars = variables;
        vars[var] = x;
        return left->evaluate(vars) - right->evaluate(vars);
    };

    // Numerical derivative
    auto df = [&](double x) {
        return (f(x + h) - f(x - h)) / (2 * h);
    };

    // Try initial guesses across the domain [-10, 10]
    for (double x0 = -10.0; x0 <= 10.0; x0 += 1.0) {
        double x = x0;
        for (int i = 0; i < max_iterations; ++i) {
            double fx = f(x);
            double dfx = df(x);
            if (std::abs(dfx) < 1e-10) break; // Avoid division by zero
            double x_new = x - fx / dfx;
            if (std::abs(x_new - x) < tolerance) {
                bool is_unique = true;
                for (double sol : solutions) {
                    if (std::abs(sol - x_new) < tolerance) {
                        is_unique = false;
                        break;
                    }
                }
                if (is_unique && std::isfinite(x_new) && std::abs(f(x_new)) < tolerance) {
                    solutions.push_back(x_new);
                }
                break;
            }
            x = x_new;
        }
    }

    std::sort(solutions.begin(), solutions.end());
    return solutions;
}