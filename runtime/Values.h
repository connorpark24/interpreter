#ifndef VALUES_H
#define VALUES_H

#include <unordered_map>
#include <string>

enum class ValueType
{
    Null,
    Number,
    Boolean,
    Object
};

struct RuntimeVal
{
    ValueType type;
    virtual ~RuntimeVal() = default;
};

struct NullVal : RuntimeVal
{
    NullVal()
    {
        type = ValueType::Null;
    }
};

struct NumberVal : RuntimeVal
{
    double value;

    NumberVal(double val)
    {
        type = ValueType::Number;
        value = val;
    }
};

struct ObjectVal : RuntimeVal
{
    std::unordered_map<std::string, RuntimeVal *> properties;

    ObjectVal()
    {
        type = ValueType::Object;
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
};

#endif // VALUES_H
