#include "Lexer.h"
#include <iostream>

const std::map<std::string, TokenType> keywords = {
    {"let", TokenType::Let},
};

Token token(std::string value = "", TokenType type)
{
    return {value, type};
}

bool isAlpha(std::string src)
{
    return (src >= "a" && src <= "z") || (src >= "A" && src <= "Z");
}

bool isInt(std::string str)
{
    char c = str[0];
    return c >= '0' && c <= '9';
}

bool isSkippable(std::string str)
{
    return str == " " || str == "\n" || str == "\t";
}

std::vector<std::string> split(const std::string &sourceCode, char delimiter)
{
    std::vector<std::string> result;
    std::string token;
    for (char c : sourceCode)
    {
        if (c == delimiter)
        {
            if (!token.empty())
            {
                result.push_back(token);
                token.clear();
            }
        }
        else
        {
            token += c;
        }
    }
    if (!token.empty())
    {
        result.push_back(token);
    }
    return result;
}

std::vector<Token> tokenize(std::string sourceCode)
{
    std::vector<Token> tokens;
    std::vector<std::string> src = split(sourceCode, ' ');

    for (size_t i = 0; i < src.size(); i++)
    {
        std::string currToken = src[i];

        if (currToken[0] == '(')
        {
            tokens.push_back(token(currToken, TokenType::OpenParen));
        }
        else if (currToken[0] == ')')
        {
            tokens.push_back(token(currToken, TokenType::CloseParen));
        }
        else if (
            currToken[0] == '+' ||
            currToken[0] == '-' ||
            currToken[0] == '*' ||
            currToken[0] == '/' ||
            currToken[0] == '%')
        {
            tokens.push_back(token(currToken, TokenType::BinaryOperator));
        }
        else if (currToken[0] == '=')
        {
            tokens.push_back(token(currToken, TokenType::Equals));
        }
        else if (isInt(currToken))
        {
            tokens.push_back(token(currToken, TokenType::Number));
        }
        else if (isAlpha(currToken))
        {
            const auto it = keywords.find(currToken);
            if (it != keywords.end())
            {
                tokens.push_back(token(currToken, it->second));
            }
            else
            {
                tokens.push_back(token(currToken, TokenType::Identifier));
            }
        }
        else if (isSkippable(currToken))
        {
        }
        else
        {
            std::cout << "Unrecognized character found in source : " << src[0] << "\n";
        }
    }

    tokens.push_back(token("EndOfFile", TokenType::EndOfFile));
    return tokens;
}
