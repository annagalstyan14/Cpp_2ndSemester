#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <memory>
#include <optional>
#include <unordered_map>

// ===== SYMBOL TABLE =====
class SymbolTable {
public:
    void set(const std::string& name, double value);
    std::optional<double> get(const std::string& name) const;
    bool contains(const std::string& name) const;
    void clear();
private:
    std::unordered_map<std::string, double> table;
};

// ===== ABSTRACT BASE NODE =====
class ExpressionNode {
public:
    virtual double evaluate(const SymbolTable& symbols) const = 0;
    virtual ~ExpressionNode() = default;
};

// ===== BINARY OPERATOR NODE =====
class BinaryOpNode : public ExpressionNode {
    std::string op;
    std::shared_ptr<ExpressionNode> left;
    std::shared_ptr<ExpressionNode> right;
public:
    BinaryOpNode(const std::string& oper,
                 std::shared_ptr<ExpressionNode> lhs,
                 std::shared_ptr<ExpressionNode> rhs)
        : op(oper), left(lhs), right(rhs) {}

    double evaluate(const SymbolTable& symbols) const override;
};

// ===== UNARY OPERATOR NODE =====
class UnaryOperatorNode : public ExpressionNode {
    std::string op;
    std::shared_ptr<ExpressionNode> operand;
public:
    UnaryOperatorNode(const std::string& oper,
                      std::shared_ptr<ExpressionNode> opnd)
        : op(oper), operand(opnd) {}

    double evaluate(const SymbolTable& symbols) const override;
};

// ===== FUNCTION NODE =====
class FunctionNode : public ExpressionNode {
    std::string funcName;
    std::shared_ptr<ExpressionNode> argument;
public:
    FunctionNode(const std::string& name,
                 std::shared_ptr<ExpressionNode> arg)
        : funcName(name), argument(arg) {}

    double evaluate(const SymbolTable& symbols) const override;
};

// ===== VARIABLE NODE =====
class VariableNode : public ExpressionNode {
    std::string name;
public:
    VariableNode(const std::string& n) : name(n) {}

    double evaluate(const SymbolTable& symbols) const override;
};

#endif // EXPRESSION_H
