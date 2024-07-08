#ifndef VALUES_H
#define VALUES_H

enum class ValueType
{
    Null,
    Number
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

#endif // VALUES_H
