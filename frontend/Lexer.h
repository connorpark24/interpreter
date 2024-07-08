#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>
#include <vector>
#include <deque>

// Define the TokenType enum
enum class TokenType
{
    Number,
    Identifier,
    String,
    Equals,
    OpenParen,
    CloseParen,
    BinaryOperator,
    Let,
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
bool isAlpha(std::string c);
bool isInt(std::string str);
bool isSkippable(std::string c);
std::deque<Token> tokenize(std::string sourceCode);

#endif // TOKEN_H
