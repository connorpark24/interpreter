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
    AssignmentExpr,

    // Literals
    Property,
    ObjectLiteral,
    NumericLiteral,
    Identifier,
    BinaryExpr,
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

    BinaryExpr(Expr *left, Expr *right, const std::string &op)
        : left(left), right(right), op(op)
    {
        this->kind = NodeType::BinaryExpr;
    }
};

struct Identifier : Expr
{
    std::string symbol;

    Identifier(const std::string &symbol)
        : symbol(symbol)
    {
        this->kind = NodeType::Identifier;
    }
};

struct NumericLiteral : Expr
{
    double value;

    NumericLiteral(double value)
        : value(value)
    {
        this->kind = NodeType::NumericLiteral;
    }
};

struct Property : Expr
{
    std::string key;
    Expr *value;

    Property(std::string key, Expr *value)
        : key(key), value(value)
    {
        this->kind = NodeType::Property;
    }
};

struct ObjectLiteral : Expr
{
    std::vector<Property *> properties;

    ObjectLiteral(std::vector<Property *> properties)
        : properties(properties)
    {
        this->kind = NodeType::ObjectLiteral;
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
