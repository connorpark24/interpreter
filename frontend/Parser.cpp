#include "Parser.h"
#include "Lexer.h"

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
        std::cerr << "Parser error: " << err << " - Expecting: " << static_cast<int>(type) << "\n";
    }
    return prev;
}

Program *Parser::produceAST(std::string sourceCode)
{
    tokens = tokenize(sourceCode);
    Program *program = new Program();

    while (not_eof())
    {
        program->body.push_back(parse_stmt());
    }

    return program;
}

Stmt *Parser::parse_stmt()
{
    switch (at().type)
    {
    case TokenType::Let:

    case TokenType::Const:
        return parse_var_declaration();
    default:
        return parse_expr();
    }
}

Stmt *Parser::parse_var_declaration()
{

    const bool isConstant = eat().type == TokenType::Const;
    const std::string identifier = expect(TokenType::Identifier, "Expected identifier name following let | const keywords. ").value;

    if (at().type == TokenType::Semicolon)
    {
        eat();
        if (isConstant)
        {
            throw std::runtime_error("Must assign value to constant expression.");
        }

        return new VarDeclaration{false, identifier, nullptr};
    }

    expect(TokenType::Equals, "Expected equals sign following identifier name in variable declaration.");

    VarDeclaration *declaration = new VarDeclaration{isConstant, identifier, parse_expr()};

    expect(TokenType::Semicolon, "Expected semicolon following variable declaration.");

    return declaration;
}

Expr *Parser::parse_expr()
{
    return parse_assignment_expr();
}

Expr *Parser::parse_assignment_expr()
{
    Expr *left = parse_additive_expr();

    if (at().type == TokenType::Equals)
    {
        eat();
        Expr *value = parse_assignment_expr();
        return new AssignmentExpr(left, value);
    }

    return left;
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

    case TokenType::Number:
        return new NumericLiteral(NodeType::NumericLiteral, std::stod(eat().value));

    case TokenType::OpenParen:
    {
        eat();
        Expr *value = parse_expr();
        expect(TokenType::CloseParen, "Unexpected token found inside parenthesised expression.");
        return value;
    }
    case TokenType::Equals:
        break;
    case TokenType::CloseParen:
        break;
    case TokenType::BinaryOperator:
        break;
    case TokenType::Let:
        break;
    case TokenType::EndOfFile:
        break;
    default:
        std::cout << "Unexpected token found during parsing: " << at().value << "\n";
        return nullptr;
    }
}
