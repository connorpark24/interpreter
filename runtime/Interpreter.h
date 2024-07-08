#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../frontend/Ast.h"
#include "Values.h"
#include "Environment.h"

RuntimeVal *evaluate(Stmt *astNode, Environment *env);
RuntimeVal *eval_program(Program *program, Environment *env);
RuntimeVal *eval_binary_expr(BinaryExpr *binop, Environment *env);
NumberVal *eval_numeric_binary_expr(NumberVal *lhs, NumberVal *rhs, const std::string &op);

#endif // INTERPRETER_H
