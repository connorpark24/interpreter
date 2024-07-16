#include "Lexer.h"

#include <iostream>
#include <deque>
#include <map>
#include <string>

const std::map<std::string, TokenType> keywords = {
    {"let", TokenType::Let},
    {"const", TokenType::Const},
    {"fn", TokenType::Fn},
    {"if", TokenType::If},
    {"else", TokenType::Else},
    {"for", TokenType::For},
    {"while", TokenType::While}};

Token token(TokenType type, std::string value = "")
{
    return {value, type};
}

bool isAlpha(const char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isInt(const char c)
{
    return c >= '0' && c <= '9';
}

bool isSkippable(const char c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

std::deque<Token> tokenize(const std::string sourceCode)
{
    std::deque<Token> tokens;
    size_t i = 0;
    const size_t numTokens = sourceCode.length();

    while (i < numTokens)
    {
        const char currToken = sourceCode[i];

        if (currToken == '(')
        {
            tokens.push_back(token(TokenType::OpenParen, "("));
        }
        else if (currToken == ')')
        {
            tokens.push_back(token(TokenType::CloseParen, ")"));
        }
        else if (currToken == '{')
        {
            tokens.push_back(token(TokenType::OpenBrace, "{"));
        }
        else if (currToken == '}')
        {
            tokens.push_back(token(TokenType::CloseBrace, "}"));
        }
        else if (currToken == '[')
        {
            tokens.push_back(token(TokenType::OpenBracket, "["));
        }
        else if (currToken == ']')
        {
            tokens.push_back(token(TokenType::CloseBracket, "]"));
        }
        else if (
            currToken == '+' ||
            currToken == '-' ||
            currToken == '*' ||
            currToken == '/' ||
            currToken == '%')
        {
            tokens.push_back(token(TokenType::BinaryOperator, std::string(1, currToken)));
        }
        else if (currToken == '=')
        {
            tokens.push_back(token(TokenType::Equals, "="));
        }
        else if (currToken == ':')
        {
            tokens.push_back(token(TokenType::Colon, ":"));
        }
        else if (currToken == ',')
        {
            tokens.push_back(token(TokenType::Comma, ","));
        }
        else if (currToken == ';')
        {
            tokens.push_back(token(TokenType::Semicolon, ";"));
        }
        else if (currToken == '.')
        {
            tokens.push_back(token(TokenType::Dot, "."));
        }
        else if (isInt(currToken))
        {
            std::string num;
            while (i < numTokens && isInt(sourceCode[i]))
            {
                num += sourceCode[i];
                i++;
            }
            i--;
            tokens.push_back(token(TokenType::Number, num));
        }
        else if (isAlpha(currToken))
        {
            std::string ident;
            while (i < numTokens && isAlpha(sourceCode[i]))
            {
                ident += sourceCode[i];
                i++;
            }
            i--;
            auto it = keywords.find(ident);
            if (it != keywords.end())
            {
                tokens.push_back(token(it->second, ident));
            }
            else
            {
                tokens.push_back(token(TokenType::Identifier, ident));
            }
        }
        else if (isSkippable(currToken))
        {
        }
        else
        {
            std::cout << "Unrecognized character found in source: " << currToken << "\n";
        }

        i++;
    }

    tokens.push_back(token(TokenType::EndOfFile, "EndOfFile"));

    return tokens;
}
