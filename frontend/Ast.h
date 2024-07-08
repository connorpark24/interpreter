#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

enum class NodeType
{
    Program,
    NumericLiteral,
    NullLiteral,
    Identifier,
    BinaryExpr,
    CallExpr,
    UnaryExpr,
    FunctionDeclaration
};

struct Stmt
{
    NodeType kind;
    virtual ~Stmt() = default;
};

struct Expr : Stmt
{
};

struct BinaryExpr : Expr
{
    Expr *left;
    Expr *right;
    std::string op;

    BinaryExpr(NodeType kind, Expr *left, Expr *right, const std::string &op)
        : left(left), right(right), op(op)
    {
        this->kind = kind;
    }
};

struct Identifier : Expr
{
    std::string symbol;

    Identifier(NodeType kind, const std::string &symbol)
        : symbol(symbol)
    {
        this->kind = kind;
    }
};

struct NullLiteral : Expr
{
    std::string value;

    NullLiteral(NodeType kind, const std::string &value)
        : value(value)
    {
        this->kind = kind;
    }
};

struct NumericLiteral : Expr
{
    double value;

    NumericLiteral(NodeType kind, double value)
        : value(value)
    {
        this->kind = kind;
    }
};

struct Program : Stmt
{
    std::vector<Stmt *> body;

    Program()
    {
        kind = NodeType::Program;
    }
};

#endif // AST_H
