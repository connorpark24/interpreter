#include "Environment.h"
#include "Values.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

RuntimeVal *getCurrentTime(std::vector<RuntimeVal *> args, Environment *scope)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    std::string formatted_time = ss.str();

    return new StringVal(formatted_time);
}

Environment createGlobalEnv()
{
    Environment env = new Environment();

    env.declareVar("true", new BooleanVal(true), true);
    env.declareVar("false", new BooleanVal(false), true);
    env.declareVar("null", new NullVal(), true);

    env.declareVar("print", new NativeFunctionVal([](std::vector<RuntimeVal *> args, Environment *scope) -> RuntimeVal *
                                                  {
                                                      for (RuntimeVal *arg : args)
                                                      {
                                                          std::cout << arg->toString() << " ";
                                                      }
                                                      std::cout << "\n";
                                                      return new NullVal(); }),
                   true);

    env.declareVar("time", new NativeFunctionVal(getCurrentTime), true);

    return env;
}

Environment::Environment(Environment *parentENV) : parent(parentENV)
{
    global = parentENV ? true : false;
}

RuntimeVal *Environment::declareVar(const std::string &varname, RuntimeVal *value, bool constant)
{
    if (variables.find(varname) != variables.end())
    {
        throw std::runtime_error("Variable already declared");
    }

    variables[varname] = value;

    if (constant)
    {
        constants.insert(varname);
    }

    return value;
}

RuntimeVal *Environment::assignVar(const std::string &varname, RuntimeVal *value)
{
    Environment *env = resolve(varname);

    if (env->constants.find(varname) != env->constants.end())
    {
        throw std::runtime_error("Cannot assign to constant");
    }

    env->variables[varname] = value;

    return value;
}

RuntimeVal *Environment::lookupVar(std::string varname)
{
    Environment *env = resolve(varname);
    return env->variables[varname];
}

Environment *Environment::resolve(const std::string &varname)
{
    if (variables.find(varname) != variables.end())
    {
        return this;
    }

    if (parent == nullptr)
    {
        std::cerr << "Variable not found: " << varname << "\n";
    }

    return parent->resolve(varname);
}
