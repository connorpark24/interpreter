#ifndef VALUES_H
#define VALUES_H

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>

// Forward declaration of Environment to resolve circular dependency
class Environment;

enum class ValueType
{
    Null,
    Number,
    Boolean,
    Object,
    NativeFn,
    Function,
    String
};

struct RuntimeVal
{
    ValueType type;
    virtual ~RuntimeVal() = default;
    virtual std::string toString() const = 0; // Add a virtual toString method
};

struct NullVal : RuntimeVal
{
    NullVal()
    {
        type = ValueType::Null;
    }
    std::string toString() const override { return "null"; }
};

struct NumberVal : RuntimeVal
{
    double value;

    NumberVal(double val)
    {
        type = ValueType::Number;
        value = val;
    }

    std::string toString() const override
    {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
};

struct ObjectVal : RuntimeVal
{
    std::unordered_map<std::string, RuntimeVal *> properties;

    ObjectVal()
    {
        type = ValueType::Object;
    }

    std::string toString() const override
    {
        std::ostringstream oss;
        oss << "{";
        for (const auto &pair : properties)
        {
            oss << pair.first << ": " << pair.second->toString() << ", ";
        }
        std::string result = oss.str();
        if (result.length() > 1)
            result.pop_back();
        if (result.length() > 1)
            result.pop_back();
        oss << "}";
        return oss.str();
    }
};

struct StringVal : RuntimeVal
{
    std::string value;

    StringVal(const std::string &val)
    {
        type = ValueType::String;
        value = val;
    }

    std::string toString() const override
    {
        return value;
    }
};

struct BooleanVal : RuntimeVal
{
    bool value;
    BooleanVal(bool val = true)
    {
        type = ValueType::Boolean;
        value = val;
    }

    std::string toString() const override
    {
        return value ? "true" : "false";
    }
};

using FunctionCall = std::function<RuntimeVal *(std::vector<RuntimeVal *>, Environment *)>;

struct NativeFunctionVal : RuntimeVal
{
    FunctionCall call;

    NativeFunctionVal(FunctionCall call) : call(call)
    {
        type = ValueType::NativeFn;
    }

    std::string toString() const override
    {
        return "<native function>";
    }
};

struct FunctionVal : RuntimeVal
{
    std::vector<Stmt *> body;
    std::string name;
    std::vector<std::string> parameters;
    Environment *declarationEnv;

    FunctionVal(std::vector<Stmt *> body, const std::string &name, std::vector<std::string> parameters, Environment *env)
        : body(body), name(name), parameters(parameters), declarationEnv(env)
    {
        type = ValueType::Function;
    }

    std::string toString() const override
    {
        return "<function " + name + ">";
    }
};

#endif // VALUES_H
