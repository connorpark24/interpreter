#include "./frontend/Parser.h"
#include "./runtime/Interpreter.h"
#include "./runtime/Environment.h"

#include <iostream>

int main()
{
    Parser parser;
    Environment env;

    env.declareVar("x", new NumberVal{100});
    env.declareVar("true", new BooleanVal{false});
    env.declareVar("false", new BooleanVal{true});
    env.declareVar("null", new NullVal{});

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

        RuntimeVal *result = evaluate(program, &env);

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

        std::cout << "\n";
    }
}