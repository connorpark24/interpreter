#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

enum class NodeType
{
    // Statements
    Program,
    VarDeclaration,
    FunctionDeclaration,

    // Expressions
    AssignmentExpr,
    MemberExpr,
    CallExpr,

    // Literals
    Property,
    ObjectLiteral,
    NumericLiteral,
    Identifier,
    BinaryExpr,
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

struct CallExpr : Expr
{
    Expr *caller;
    std::vector<Expr *> args;

    CallExpr(Expr *caller, std::vector<Expr *> args)
        : caller(caller), args(args)
    {
        this->kind = NodeType::CallExpr;
    }
};

struct MemberExpr : Expr
{
    Expr *object;
    Expr *property;
    bool computed;

    MemberExpr(Expr *object, Expr *property, bool computed)
        : object(object), property(property), computed(computed)
    {
        this->kind = NodeType::MemberExpr;
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

struct FunctionDeclaration : Stmt
{
    std::vector<Stmt *> body;
    std::string name;
    std::vector<std::string> parameters;

    FunctionDeclaration(std::vector<Stmt *> body, const std::string &name, std::vector<std::string> parameters)
        : body(body), name(name), parameters(parameters)
    {
        kind = NodeType::FunctionDeclaration;
    }
};

#endif // AST_H
