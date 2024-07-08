#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Values.h"
#include <unordered_map>
#include <string>
#include <stdexcept>

class Environment
{
private:
    Environment *parent;
    std::unordered_map<std::string, RuntimeVal *> variables;

public:
    Environment(Environment *parentENV = nullptr);

    RuntimeVal *declareVar(const std::string &name, RuntimeVal *value);
    RuntimeVal *lookupVar(std::string varname);
    RuntimeVal *assignVar(const std::string &varname, RuntimeVal *value);
    Environment *resolve(const std::string &varname);
};

#endif
