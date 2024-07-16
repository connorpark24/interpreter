#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>
#include <vector>
#include <deque>

enum class TokenType
{
    // Literal Types
    Number,
    Identifier,

    // Operators
    Equals,
    BinaryOperator,

    // Delimiters
    OpenParen,
    CloseParen,
    OpenBrace,
    CloseBrace,
    OpenBracket,
    CloseBracket,
    Semicolon,
    Comma,
    Dot,
    Colon,

    // Keywords
    Let,
    Const,
    Fn,
    If,
    Else,
    For,
    While,

    // Signal end of file
    EndOfFile
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
