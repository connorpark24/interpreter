#include "../frontend/Parser.h"

#include "Interpreter.h"

#include <iostream>

RuntimeVal *eval_program(Program *program, Environment *env)
{
    RuntimeVal *lastEvaluated = new NullVal();

    for (Stmt *statement : program->body)
    {
        lastEvaluated = evaluate(statement, env);
    }
    return lastEvaluated;
}

RuntimeVal *eval_binary_expr(BinaryExpr *binop, Environment *env)
{
    RuntimeVal *lhs = evaluate(binop->left, env);
    RuntimeVal *rhs = evaluate(binop->right, env);

    if (lhs->type == ValueType::Number && rhs->type == ValueType::Number)
    {
        return eval_numeric_binary_expr(
            (NumberVal *)lhs,
            (NumberVal *)rhs,
            binop->op);
    }

    return new NullVal();
}

NumberVal *eval_numeric_binary_expr(
    NumberVal *lhs,
    NumberVal *rhs,
    const std::string &op)
{
    double result = 0;
    if (op == "+")
        result = lhs->value + rhs->value;
    else if (op == "-")
        result = lhs->value - rhs->value;
    else if (op == "*")
        result = lhs->value * rhs->value;
    else if (op == "/")
        result = lhs->value / rhs->value;
    else
        result = static_cast<int>(lhs->value) % static_cast<int>(rhs->value);

    return new NumberVal{result};
}

RuntimeVal *eval_identifier(Identifier *ident, Environment *env)
{
    RuntimeVal *val = env->lookupVar(ident->symbol);
    return val;
}

RuntimeVal *evaluate(Stmt *astNode, Environment *env)
{
    switch (astNode->kind)
    {
    case NodeType::NumericLiteral:
    {
        NumericLiteral *numLiteral = static_cast<NumericLiteral *>(astNode);
        return new NumberVal{numLiteral->value};
    }
    case NodeType::Identifier:
        return eval_identifier(static_cast<Identifier *>(astNode), env);
    case NodeType::BinaryExpr:
        return eval_binary_expr(static_cast<BinaryExpr *>(astNode), env);
    case NodeType::Program:
        return eval_program(static_cast<Program *>(astNode), env);
    default:
        std::cerr << "Unknown AST Node\n";
        exit(1);
    }
}
