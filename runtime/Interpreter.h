#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../frontend/Ast.h"
#include "Values.h"
#include "Environment.h"

RuntimeVal *evaluate(Stmt *astNode, Environment *env);
RuntimeVal *eval_program(Program *program, Environment *env);
RuntimeVal *eval_binary_expr(BinaryExpr *binop, Environment *env);
NumberVal *eval_numeric_binary_expr(NumberVal *lhs, NumberVal *rhs, const std::string &op);
RuntimeVal *eval_identifier(Identifier *ident, Environment *env);
RuntimeVal *eval_assignment(AssignmentExpr *node, Environment *env);
RuntimeVal *eval_object_expr(ObjectLiteral *obj, Environment *env);
RuntimeVal *eval_call_expr(CallExpr *obj, Environment *env);
RuntimeVal *eval_var_declaration(VarDeclaration *declaration, Environment *env);
RuntimeVal *eval_function_declaration(FunctionDeclaration *declaration, Environment *env);
RuntimeVal *eval_for_stmt(ForStmt *stmt, Environment *env);
RuntimeVal *eval_while_stmt(WhileStmt *stmt, Environment *env);
RuntimeVal *eval_if_stmt(IfStmt *stmt, Environment *env);

#endif // INTERPRETER_H