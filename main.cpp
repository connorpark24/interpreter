#include "./frontend/Parser.h"
#include "./runtime/Interpreter.h"
#include "./runtime/Environment.h"

#include <iostream>
#include <sstream>

int main()
{
    Parser parser;
    Environment env = createGlobalEnv();

    std::cout << "\nRepl v0.1\n";

    std::stringstream input_stream;
    input_stream << std::cin.rdbuf();
    std::string input = input_stream.str();

    Program *program = parser.produceAST(input);

    RuntimeVal *result = evaluate(program, &env);

    std::cout << "\n";
}