#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>
#include <vector>

// Define the TokenType enum
enum class TokenType
{
    Null,
    Number,
    Identifier,
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
Token token(std::string value, TokenType type);
bool isAlpha(std::string c);
bool isInt(std::string str);
bool isSkippable(std::string c);
std::vector<Token> tokenize(const std::string &sourceCode);

#endif // TOKEN_H
