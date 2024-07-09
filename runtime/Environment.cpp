#include "Environment.h"

Environment::Environment(Environment *parentENV) : parent(parentENV) {}

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
        throw std::runtime_error("Variable not found");
    }

    return parent->resolve(varname);
}
