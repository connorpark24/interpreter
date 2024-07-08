#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Ast.h"
#include "Values.h"

RuntimeVal *evaluate(Stmt *astNode);
RuntimeVal *eval_program(Program *program);
RuntimeVal *eval_binary_expr(BinaryExpr *binop);
NumberVal *eval_numeric_binary_expr(NumberVal *lhs, NumberVal *rhs, const std::string &op);

#endif // INTERPRETER_H
