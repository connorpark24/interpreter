#include "../frontend/Parser.h"

#include "Interpreter.h"

#include <iostream>
#include <algorithm>

bool isTruthy(RuntimeVal *val)
{
    if (val->type == ValueType::Boolean)
    {
        return static_cast<BooleanVal *>(val)->value;
    }
    else if (val->type == ValueType::Null)
    {
        return false;
    }
    else if (val->type == ValueType::Number)
    {
        return static_cast<NumberVal *>(val)->value != 0;
    }
    else if (val->type == ValueType::String)
    {
        return !static_cast<StringVal *>(val)->value.empty();
    }
    else if (val->type == ValueType::Object)
    {
        return true;
    }
    else if (val->type == ValueType::NativeFn)
    {
        return true;
    }
    else if (val->type == ValueType::Function)
    {
        return true;
    }

    return false;
}

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

RuntimeVal *eval_assignment(AssignmentExpr *node, Environment *env)
{
    if (node->assignee->kind != NodeType::Identifier)
    {
        throw std::runtime_error("Left hand side of assignment must be an identifier");
    }

    std::string varname = static_cast<Identifier *>(node->assignee)->symbol;
    return env->assignVar(varname, evaluate(node->value, env));
}

RuntimeVal *eval_var_declaration(VarDeclaration *declaration, Environment *env)
{
    RuntimeVal *val = declaration->value ? evaluate(declaration->value, env) : new NullVal();
    return env->declareVar(declaration->identifier, val, declaration->constant);
}

RuntimeVal *eval_object_expr(ObjectLiteral *obj, Environment *env)
{
    ObjectVal *object = new ObjectVal();

    for (auto &prop : obj->properties)
    {
        RuntimeVal *runtimeVal = prop->value ? evaluate(prop->value, env) : env->lookupVar(prop->key);

        object->properties[prop->key] = runtimeVal;
    }

    return object;
}

RuntimeVal *eval_call_expr(CallExpr *expr, Environment *env)
{
    std::vector<RuntimeVal *> args(expr->args.size());

    std::transform(expr->args.begin(), expr->args.end(), args.begin(), [env](Expr *arg)
                   { return evaluate(arg, env); });

    RuntimeVal *fn = evaluate(expr->caller, env);

    if (fn->type == ValueType::NativeFn)

    {
        RuntimeVal *result = (static_cast<NativeFunctionVal *>(fn))->call(args, env);
        return result;
    }

    if (fn->type == ValueType::Function)
    {
        FunctionVal *function = static_cast<FunctionVal *>(fn);
        Environment *scope = new Environment(function->declarationEnv);

        for (size_t i = 0; i < function->parameters.size(); i++)
        {
            scope->declareVar(function->parameters[i], args[i], false);
        }

        RuntimeVal *result;

        for (Stmt *stmt : function->body)
        {
            result = evaluate(stmt, scope);
        }

        return result;
    }

    throw std::runtime_error("Attempted to call a non-function");
}

RuntimeVal *eval_function_declaration(FunctionDeclaration *declaration, Environment *env)
{
    FunctionVal *function = new FunctionVal(declaration->body, declaration->name, declaration->parameters, env);

    return env->declareVar(declaration->name, function, true);
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
    case NodeType::ObjectLiteral:
        return eval_object_expr(static_cast<ObjectLiteral *>(astNode), env);
    case NodeType::AssignmentExpr:
        return eval_assignment(static_cast<AssignmentExpr *>(astNode), env);
    case NodeType::BinaryExpr:
        return eval_binary_expr(static_cast<BinaryExpr *>(astNode), env);
    case NodeType::CallExpr:
        return eval_call_expr(static_cast<CallExpr *>(astNode), env);
    case NodeType::Program:
        return eval_program(static_cast<Program *>(astNode), env);
    case NodeType::VarDeclaration:
        return eval_var_declaration(static_cast<VarDeclaration *>(astNode), env);
    case NodeType::FunctionDeclaration:
        return eval_function_declaration(static_cast<FunctionDeclaration *>(astNode), env);
    default:
        std::cerr << "Unknown AST Node\n";
        exit(1);
    }
}
