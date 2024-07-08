#include "Lexer.h"

#include <iostream>

const std::map<std::string, TokenType> keywords = {
    {"let", TokenType::Let},
};

Token token(TokenType type, std::string value = "")
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

std::vector<std::string> split(std::string sourceCode, char delimiter)
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

std::deque<Token> tokenize(std::string sourceCode)
{
    std::deque<Token> tokens;
    std::vector<std::string> src = split(sourceCode, ' ');
    const size_t numTokens = src.size();

    std::cout << "Tokens:\n";

    for (size_t i = 0; i < numTokens; i++)
    {
        std::string currToken = src[i];
        std::cout << currToken << "\n";

        if (currToken[0] == '(')
        {
            tokens.push_back(token(TokenType::OpenParen, currToken));
        }
        else if (currToken[0] == ')')
        {
            tokens.push_back(token(TokenType::CloseParen, currToken));
        }
        else if (
            currToken[0] == '+' ||
            currToken[0] == '-' ||
            currToken[0] == '*' ||
            currToken[0] == '/' ||
            currToken[0] == '%')
        {
            tokens.push_back(token(TokenType::BinaryOperator, currToken));
        }
        else if (currToken[0] == '=')
        {
            tokens.push_back(token(TokenType::Equals, currToken));
        }
        else if (isInt(currToken))
        {
            tokens.push_back(token(TokenType::Number, currToken));
        }
        else if (isAlpha(currToken))
        {
            const auto it = keywords.find(currToken);
            if (it != keywords.end())
            {
                tokens.push_back(token(it->second, currToken));
            }
            else
            {
                tokens.push_back(token(TokenType::Identifier, currToken));
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

    tokens.push_back(token(TokenType::EndOfFile, "EndOfFile"));

    return tokens;
}
