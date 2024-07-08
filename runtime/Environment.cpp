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
    auto it = variables.find(varname);
    if (it != variables.end())
    {
        it->second = value;
        return value;
    }
    if (parent != nullptr)
    {
        return parent->assignVar(varname, value);
    }
    throw std::runtime_error("Variable not declared");
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
