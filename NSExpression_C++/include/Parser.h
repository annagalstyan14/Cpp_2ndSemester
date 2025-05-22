#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

class ExpressionNode {
public:
    virtual double evaluate(const std::unordered_map<std::string, double>& variables) = 0;
    virtual bool isEquation() const { return false; }
    virtual ~ExpressionNode() = default;
};

class NumberNode : public ExpressionNode {
public:
    NumberNode(double value);
    double evaluate(const std::unordered_map<std::string, double>& variables) override;
private:
    double value;
};

class VariableNode : public ExpressionNode {
public:
    VariableNode(const std::string& name);
    double evaluate(const std::unordered_map<std::string, double>& variables) override;
private:
    std::string name;
};

class BinaryOpNode : public ExpressionNode {
public:
    BinaryOpNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right, const std::string& op);
    double evaluate(const std::unordered_map<std::string, double>& variables) override;
private:
    std::shared_ptr<ExpressionNode> left, right;
    std::string op;
};

class UnaryOpNode : public ExpressionNode {
public:
    UnaryOpNode(std::shared_ptr<ExpressionNode> operand, const std::string& op);
    double evaluate(const std::unordered_map<std::string, double>& variables) override;
private:
    std::shared_ptr<ExpressionNode> operand;
    std::string op;
};

class FunctionNode : public ExpressionNode {
public:
    FunctionNode(const std::string& name, const std::vector<std::shared_ptr<ExpressionNode>>& args);
    double evaluate(const std::unordered_map<std::string, double>& variables) override;
    std::string getName() const { return name; }
    std::vector<std::shared_ptr<ExpressionNode>> getArgs() const { return args; }
private:
    std::string name;
    std::vector<std::shared_ptr<ExpressionNode>> args;
};

class EquationNode : public ExpressionNode {
public:
    EquationNode(std::shared_ptr<ExpressionNode> left, std::shared_ptr<ExpressionNode> right);
    double evaluate(const std::unordered_map<std::string, double>& variables) override;
    bool isEquation() const override { return true; }
    double solveFor(const std::string& var, const std::unordered_map<std::string, double>& variables);
    std::vector<double> solveNonLinear(const std::string& var, const std::unordered_map<std::string, double>& variables);
private:
    std::shared_ptr<ExpressionNode> left, right;
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens, std::unordered_map<std::string, double>& variables);
    std::shared_ptr<ExpressionNode> parse();
private:
    std::vector<Token> tokens;
    size_t pos;
    std::unordered_map<std::string, double>& variables;
    Token currentToken() const;
    void advance();
    std::shared_ptr<ExpressionNode> parseExpression();
    std::shared_ptr<ExpressionNode> parseTerm();
    std::shared_ptr<ExpressionNode> parseFactor();
    std::shared_ptr<ExpressionNode> parsePrimary();
};

#endif // PARSER_H