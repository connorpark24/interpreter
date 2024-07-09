#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>
#include <vector>
#include <deque>

// Define the TokenType enum
enum class TokenType
{
    // Literal Types
    Number,
    Identifier,

    // Operators
    Equals,
    OpenParen,
    CloseParen,
    OpenBrace,
    CloseBrace,
    BinaryOperator,
    Semicolon,
    Comma,
    Colon,

    // Keywords
    Let,
    Const,

    // Signal end of file
    EndOfFile,
};

// Define the Token struct
struct Token
{
    std::string value;
    TokenType type;
};

// Token utility functions
Token token(TokenType type, std::string value);
bool isAlpha(const char c);
bool isInt(const char str);
bool isSkippable(const char c);
std::deque<Token> tokenize(const std::string sourceCode);

#endif // TOKEN_H
