#include "Environment.h"

Environment::Environment(Environment *parentENV) : parent(parentENV) {}

RuntimeVal *Environment::declareVar(const std::string &name, RuntimeVal *value)
{
    if (variables.find(name) != variables.end())
    {
        throw std::runtime_error("Variable already declared");
    }

    variables[name] = value;
    return value;
}

RuntimeVal *Environment::assignVar(const std::string &varname, RuntimeVal *value)
{
    Environment *env = resolve(varname);
    env->variables[varname] = value;

    return value;
}


RuntimeVal * Environment::lookupVar (std::string varname) {
    Environment * env = resolve(varname);
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
