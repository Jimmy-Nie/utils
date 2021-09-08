#pragma once

#include <vector>
#include <string>
#include "iodata/databuffer.h"

namespace jm
{

/// \brief Value
class Value
{
public:
    enum Type
    {
        null,
        integer,
        uinteger,
        character,
        ucharacter,
        string,
        boolean,
        number,
        complex,
        binary
    };

    Value();

    /// \brief Constructs a Value
    ///
    /// \param value = value
    Value(int value);

    /// \brief Constructs a Value
    ///
    /// \param value = value
    Value(unsigned int value);

    /// \brief Constructs a Value
    ///
    /// \param value = value
    Value(char value);

    /// \brief Constructs a Value
    ///
    /// \param value = value
    Value(unsigned char value);

    /// \brief Constructs a Value
    ///
    /// \param value = value
    Value(float value);

    /// \brief Constructs a Value
    ///
    /// \param value = String
    Value(const std::string &value);

    /// \brief Constructs a Value
    ///
    /// \param str = char
    Value(const char *str);

    /// \brief Constructs a Value
    ///
    /// \param value = bool
    Value(bool value);

    /// \brief Constructs a Value
    ///
    /// \param value = DataBuffer
    Value(const DataBuffer &value);

    /// \brief Constructs a Value
    ///
    /// \param type = Type
    Value(Type type);

    /// \brief Get Type
    ///
    /// \return type
    Type get_type() const;

    /// \brief Is Null
    ///
    /// \return true = null
    bool is_null() const;

    /// \brief Is Uinteger
    ///
    /// \return true = uinteger
    bool is_uinteger() const;

    /// \brief Is Integer
    ///
    /// \return true = integer
    bool is_integer() const;

    /// \brief Is Ucharacter
    ///
    /// \return true = ucharacter
    bool is_ucharacter() const;

    /// \brief Is Character
    ///
    /// \return true = character
    bool is_character() const;

    /// \brief Is Number
    ///
    /// \return true = number
    bool is_number() const;

    /// \brief Is String
    ///
    /// \return true = string
    bool is_string() const;

    /// \brief Is Boolean
    ///
    /// \return true = boolean
    bool is_boolean() const;

    /// \brief Is Binary
    ///
    /// \return true = binary
    bool is_binary() const;

    /// \brief Is Complex
    ///
    /// \return true = complex
    bool is_complex() const;

    unsigned int get_member_count() const;
    const Value &get_member(unsigned int index) const;

    /// \brief Add member
    ///
    /// \param value = Event Value
    void add_member(const Value &value);

    /// \brief Set member
    ///
    /// \param index = value
    /// \param value = Event Value
    void set_member(unsigned int index, const Value &value);

    /// \brief To unsigned integer
    ///
    /// \return unsigned int
    unsigned int get_uinteger() const;

    /// \brief To integer
    ///
    /// \return int
    int get_integer() const;

    /// \brief To unsigned character
    ///
    /// \return unsigned char
    unsigned int get_ucharacter() const;

    /// \brief To character
    ///
    /// \return char
    int get_character() const;

    /// \brief To number
    ///
    /// \return float
    float get_number() const;

    /// \brief To string
    ///
    /// \return String
    std::string get_string() const;

    /// \brief To boolean
    ///
    /// \return bool
    bool get_boolean() const;

    /// \brief To binary
    ///
    /// \return binary
    DataBuffer get_binary() const;

    inline operator unsigned int() const { return get_uinteger(); }
    inline operator int() const { return get_integer(); }
    inline operator unsigned char() const { return get_ucharacter(); }
    inline operator float() const { return get_number(); }
    inline operator std::string() const { return get_string(); }
    inline operator bool() const { return get_boolean(); }
    inline operator DataBuffer() const { return get_binary(); }

    /// Helper function to obtain a string representation of an Value object.
    static std::string to_string(const Value &);

private:
    /// \brief Throw if not complex
    void throw_if_not_complex() const;

    Type type;
    union
    {
        int value_int;
        unsigned int value_uint;
        char value_char;
        unsigned char value_uchar;
        float value_float;
        bool value_bool;
    };
    std::string value_string;
    DataBuffer value_binary;
    std::vector<Value> value_complex;
};

}
