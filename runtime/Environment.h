#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <stdexcept>

struct RuntimeVal;

class Environment
{
private:
    Environment *parent;
    bool global;
    std::unordered_map<std::string, RuntimeVal *> variables;
    std::unordered_set<std::string> constants;

public:
    Environment(Environment *parentENV = nullptr);

    RuntimeVal *declareVar(const std::string &name, RuntimeVal *value, bool constant);
    RuntimeVal *lookupVar(std::string varname);
    RuntimeVal *assignVar(const std::string &varname, RuntimeVal *value);
    Environment *resolve(const std::string &varname);
};

Environment createGlobalEnv();

#endif
