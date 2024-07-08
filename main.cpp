#include "Parser.h"
#include "Interpreter.h"

#include <iostream>

int main(int argc, char **argv)
{
    Parser parser;

    std::cout << "\nRepl v0.1\n";

    while (true)
    {
        std::cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit")
        {
            break;
        }

        Program *program = parser.produceAST(input);

        RuntimeVal *result = evaluate(program);

        switch (result->type)
        {
        case ValueType::Null:
            std::cout << "null";
            break;
        case ValueType::Number:
            std::cout << static_cast<NumberVal *>(result)->value;
            break;
        default:
            std::cout << "unknown";
        }
    }
}