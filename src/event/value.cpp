#include "value.h"
#include "exception/exception.h"

namespace jm
{
Value::Value()
    : type(null), value_int(0)
{
}

Value::Value(int value)
    : type(integer), value_int(value)
{
}

Value::Value(unsigned int value)
    : type(uinteger), value_uint(value)
{
}

Value::Value(char value)
    : type(character), value_char(value)
{
}

Value::Value(unsigned char value)
    : type(ucharacter), value_uchar(value)
{
}

Value::Value(float value)
    : type(number), value_float(value)
{
}

Value::Value(const std::string& value)
    : type(string), value_string(value)
{
}

Value::Value(const char *value)
    : type(string), value_string(value)
{
}

Value::Value(bool value)
    : type(boolean), value_bool(value)
{
}

Value::Value(const DataBuffer &value)
    : type(binary), value_binary(value)
{
}

Value::Value(Type type)
    : type(type), value_int(0)
{
}

Value::Type Value::get_type() const
{
    return type;
}

bool Value::is_null() const
{
    return type == null;
}

bool Value::is_uinteger() const
{
    return type == uinteger;
}

bool Value::is_integer() const
{
    return type == integer;
}

bool Value::is_ucharacter() const
{
    return type == ucharacter;
}

bool Value::is_character() const
{
    return type == character;
}

bool Value::is_number() const
{
    return type == number;
}

bool Value::is_string() const
{
    return type == string;
}

bool Value::is_boolean() const
{
    return type == boolean;
}

bool Value::is_binary() const
{
    return type == binary;
}

bool Value::is_complex() const
{
    return type == complex;
}

unsigned int Value::get_member_count() const
{
    throw_if_not_complex();
    return value_complex.size();
}

const Value &Value::get_member(unsigned int index) const
{
    throw_if_not_complex();
    return value_complex.at(index);
}

void Value::add_member(const Value &value)
{
    throw_if_not_complex();
    value_complex.push_back(value);
}

void Value::set_member(unsigned int index, const Value &value)
{
    throw_if_not_complex();
    value_complex.at(index) = value;
}

void Value::throw_if_not_complex() const
{
    if (type != complex)
        throw Exception("Value is not a complex type");
}

unsigned int Value::get_uinteger() const
{
    if (is_uinteger())
        return value_uint;
    else
        throw Exception("Value is not an unsigned integer");
}

int Value::get_integer() const
{
    if (is_integer())
        return value_int;
    else
        throw Exception("Value is not an integer");
}

unsigned int Value::get_ucharacter() const
{
    if (is_ucharacter())
        return value_uchar;
    else
        throw Exception("Value is not an unsigned character");
}

int Value::get_character() const
{
    if (is_character())
        return value_char;
    else
        throw Exception("Value is not a character");
}

float Value::get_number() const
{
    if (is_number())
        return value_float;
    else
        throw Exception("Value is not a floating point number");
}

std::string Value::get_string() const
{
    if (is_string())
        return value_string;
    else
        throw Exception("Value is not a string");
}

bool Value::get_boolean() const
{
    if (is_boolean())
        return value_bool;
    else
        throw Exception("Value is not a boolean");
}

DataBuffer Value::get_binary() const
{
    if (is_binary())
        return value_binary;
    else
        throw Exception("Value is not a binary");
}

std::string Value::to_string(const Value &v)
{
    switch (v.get_type())
    {
        case Value::null:
            return "null";
        case Value::integer:
            return std::to_string(v.get_integer());
        case Value::uinteger:
            return std::to_string(v.get_uinteger());
        case Value::character:
            return std::to_string(static_cast<int>(v.get_character()));
        case Value::ucharacter:
            return std::to_string(static_cast<unsigned int>(v.get_ucharacter()));
        case Value::string:
            return "\"" + v.get_string() + "\"";
        case Value::boolean:
            return v.get_boolean() ? "true" : "false";
        case Value::number:
            return std::to_string(v.get_number());
        case Value::complex:
        {
            std::string str;
            str += "[";
            for (unsigned int j = 0; j < v.get_member_count(); j++)
            {
                if (j > 0)
                    str += ",";
                str += to_string(v.get_member(j));
            }
            str += "]";
            return str;
        }
        default:
            return "??" + std::to_string(v.get_type());
    }
}
}