#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

enum class NodeType
{
    // Statements
    Program,
    VarDeclaration,
    // Expressions
    NumericLiteral,
    AssignmentExpr,
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

struct AssignmentExpr : Expr
{
    Expr *assignee;
    Expr *value;

    AssignmentExpr(Expr *assignee, Expr *value)
        : assignee(assignee), value(value)
    {
        this->kind = NodeType::AssignmentExpr;
    }
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

struct VarDeclaration : Stmt
{
    bool constant;
    std::string identifier;
    Expr *value;
    VarDeclaration(bool constant, const std::string identifier, Expr *value)
        : constant(constant), identifier(identifier), value(value)
    {
        kind = NodeType::VarDeclaration;
    }
};

#endif // AST_H
