#include "Parser.h"

bool Parser::not_eof()
{
    return tokens[0].type != TokenType::EndOfFile;
}

Token Parser::at()
{
    return tokens[0];
}

Token Parser::eat()
{
    const Token prev = tokens[0];
    tokens.pop_front();
    return prev;
}

Token Parser::expect(TokenType type, const std::string &err)
{
    const Token prev = tokens[0];
    tokens.pop_front();
    if (prev.type != type)
    {
        std::cout << "Parser error: " << err << " - Expecting: " << static_cast<int>(type) << "\n";
    }
    return prev;
}

Stmt *Parser::parse_stmt()
{
    return parse_expr();
}

Expr *Parser::parse_expr()
{
    return parse_additive_expr();
}

Expr *Parser::parse_additive_expr()
{
    Expr *left = parse_multiplicative_expr();

    while (at().value == "+" || at().value == "-")
    {
        std::string op = eat().value;
        Expr *right = parse_multiplicative_expr();
        left = new BinaryExpr(NodeType::BinaryExpr, left, right, op);
    }

    return left;
}

Expr *Parser::parse_multiplicative_expr()
{
    Expr *left = parse_primary_expr();
    while (at().value == "/" || at().value == "*" || at().value == "%")
    {
        std::string op = eat().value;
        Expr *right = parse_primary_expr();
        left = new BinaryExpr(NodeType::BinaryExpr, left, right, op);
    }

    return left;
}

Expr *Parser::parse_primary_expr()
{
    TokenType tokenType = at().type;

    switch (tokenType)
    {
    case TokenType::Identifier:
        return new Identifier(NodeType::Identifier, eat().value);

    case TokenType::Null:
        eat();
        return new NullLiteral(NodeType::NullLiteral, "null");

    case TokenType::Number:
        return new NumericLiteral(NodeType::NumericLiteral, std::stod(eat().value));

    case TokenType::OpenParen:
    {
        eat();
        Expr *value = parse_expr();
        expect(TokenType::CloseParen, "Unexpected token found inside parenthesised expression.");
        return value;
    }
    default:
        std::cout << "Unexpected token found during parsing: " << at().value << "\n";
        return nullptr;
    }
}

Program *Parser::produceAST(const std::string &sourceCode)
{
    tokens = std::deque<Token>(tokenize(sourceCode).begin(), tokenize(sourceCode).end());
    Program *program = new Program();

    while (not_eof())
    {
        program->body.push_back(parse_stmt());
    }

    return program;
}
