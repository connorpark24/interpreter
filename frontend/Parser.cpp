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
    Expr *left = parse_object_expr();

    if (at().type == TokenType::Equals)
    {
        eat();
        Expr *value = parse_assignment_expr();
        return new AssignmentExpr(left, value);
    }

    return left;
}

Expr *Parser::parse_object_expr()
{
    if (at().type != TokenType::OpenBrace)
    {
        return parse_additive_expr();
    }

    eat();

    std::vector<Property *> properties;

    while (not_eof() && at().type != TokenType::CloseBrace)
    {
        std::string key = expect(TokenType::Identifier, "Expected identifier key in object literal.").value;

        if (at().type == TokenType::Comma)
        {
            eat();
            properties.push_back(new Property{key, nullptr});
            continue;
        }
        else if (at().type == TokenType::CloseBrace)
        {
            properties.push_back(new Property{key, nullptr});
            continue;
        }

        expect(TokenType::Colon, "Expected colon following key in object literal.");
        Expr *value = parse_expr();

        properties.push_back(new Property{key, value});

        if (at().type != TokenType::CloseBrace)
        {
            expect(TokenType::Comma, "Expected comma following property in object literal.");
        }
    }

    expect(TokenType::CloseBrace, "Expected closing brace following object literal.");

    return new ObjectLiteral(properties);
}

Expr *Parser::parse_additive_expr()
{
    Expr *left = parse_multiplicative_expr();

    while (at().value == "+" || at().value == "-")
    {
        std::string op = eat().value;
        Expr *right = parse_multiplicative_expr();
        left = new BinaryExpr(left, right, op);
    }

    return left;
}

Expr *Parser::parse_multiplicative_expr()
{
    Expr *left = parse_call_member_expr();
    while (at().value == "/" || at().value == "*" || at().value == "%")
    {
        std::string op = eat().value;
        Expr *right = parse_call_member_expr();
        left = new BinaryExpr(left, right, op);
    }

    return left;
}

Expr *Parser::parse_call_member_expr()
{
    Expr *member = parse_member_expr();

    if (at().type == TokenType::OpenParen)
    {
        return parse_call_expr(member);
    }

    return member;
}

Expr *Parser::parse_call_expr(Expr *caller)
{
    Expr *call_expr = new CallExpr(caller, parse_args());

    if (at().type == TokenType::OpenParen)
    {
        call_expr = parse_call_expr(call_expr);
    }

    return call_expr;
}

std::vector<Expr *> Parser::parse_args()
{
    expect(TokenType::OpenParen, "Expected open parenthesis following function call.");

    std::vector<Expr *> args = (at().type == TokenType::CloseParen) ? std::vector<Expr *>() : parse_arguments_list();

    expect(TokenType::CloseParen, "Expected closing parenthesis following function call.");

    return args;
}

std::vector<Expr *> Parser::parse_arguments_list()
{
    std::vector<Expr *> args = {parse_assignment_expr()};

    while (at().type == TokenType::Comma)
    {
        eat();
        args.push_back(parse_assignment_expr());
    }

    return args;
}

Expr *Parser::parse_member_expr()
{
    Expr *object = parse_primary_expr();

    while (at().type == TokenType::Dot || at().type == TokenType::OpenBracket)
    {
        Token op = eat();
        Expr *property;
        bool computed;

        if (op.type == TokenType::Dot)
        {
            computed = false;
            property = parse_primary_expr();

            if (property->kind != NodeType::Identifier)
            {
                throw std::runtime_error("Expected identifier following dot operator.");
            }
        }
        else
        {
            computed = true;
            property = parse_expr();
            expect(TokenType::CloseBracket, "Expected closing bracket following computed property.");
        }

        object = new MemberExpr{object, property, computed};
    }

    return object;
}

Expr *Parser::parse_primary_expr()
{

    TokenType tokenType = at().type;

    switch (tokenType)
    {
    case TokenType::Identifier:
        return new Identifier(eat().value);

    case TokenType::Number:
        return new NumericLiteral(std::stod(eat().value));

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
