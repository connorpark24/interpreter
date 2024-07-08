#include "Parser.h"
#include "Interpreter.h"
#include "Values.h"

RuntimeVal *eval_program(Program *program)
{
    RuntimeVal *lastEvaluated = new NullVal();

    for (Stmt *statement : program->body)
    {
        lastEvaluated = evaluate(statement);
    }
    return lastEvaluated;
}

RuntimeVal *eval_binary_expr(BinaryExpr *binop)
{
    RuntimeVal *lhs = evaluate(binop->left);
    RuntimeVal *rhs = evaluate(binop->right);

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

RuntimeVal *evaluate(Stmt *astNode)
{
    switch (astNode->kind)
    {
    case NodeType::NumericLiteral:
    {
        NumericLiteral *numLiteral = static_cast<NumericLiteral *>(astNode);
        return new NumberVal{numLiteral->value};
    }
    case NodeType::NullLiteral:
        return new NullVal();
    case NodeType::BinaryExpr:
        return eval_binary_expr(static_cast<BinaryExpr *>(astNode));
    case NodeType::Program:
        return eval_program(static_cast<Program *>(astNode));
    default:
        std::cerr << "Unknown AST Node\n";
        exit(1);
    }
}
