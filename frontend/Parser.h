#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Ast.h"

#include <vector>
#include <deque>
#include <string>
#include <iostream>

class Parser
{
private:
    std::deque<Token> tokens;

    bool not_eof();
    Token at();
    Token eat();
    Token expect(TokenType type, const std::string &err);

    Stmt *parse_stmt();
    Stmt *parse_var_declaration();
    Stmt *parse_fn_declaration();
    Stmt *parse_for_stmt();
    Stmt *parse_while_stmt();
    Stmt *parse_if_stmt();
    Expr *parse_assignment_expr();
    Expr *parse_object_expr();
    Expr *parse_expr();
    Expr *parse_additive_expr();
    Expr *parse_multiplicative_expr();
    Expr *parse_call_member_expr();
    Expr *parse_call_expr(Expr *caller);
    std::vector<Expr *> parse_args();
    std::vector<Expr *> parse_arguments_list();
    Expr *parse_member_expr();
    Expr *parse_primary_expr();

public:
    Program *produceAST(std::string sourceCode);
};

#endif // PARSER_H
