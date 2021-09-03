#pragma once

#include <memory>

namespace jm
{

class DataBuffer_Impl;

/// \brief General purpose data buffer.
class DataBuffer
{
public:
    /// \brief Constructs a data buffer of 0 size.
    DataBuffer();
    DataBuffer(size_t size);
    DataBuffer(const DataBuffer &copy);
    DataBuffer(const void *data, size_t size);
    DataBuffer(const DataBuffer &data, size_t pos, size_t size);
    ~DataBuffer();

    /// \brief Returns a pointer to the data.
    char *get_data();

    const char *get_data() const;

    template<typename Type>
    Type *get_data() { return reinterpret_cast<Type*>(get_data()); }

    template<typename Type>
    const Type *get_data() const { return reinterpret_cast<const Type*>(get_data()); }

    /// \brief Returns the size of the data.
    size_t get_size() const;

    /// \brief Returns the capacity of the data buffer object.
    size_t get_capacity() const;

    /// \brief Returns a char in the buffer.
    char &operator[](size_t i);
    const char &operator[](size_t i) const;

    /// \brief Returns true if the buffer is 0 in size.
    bool is_null() const;

    DataBuffer &operator =(const DataBuffer &copy);

    /// \brief Resize the buffer.
    void set_size(size_t size);

    /// \brief Preallocate enough memory.
    void set_capacity(size_t capacity);

private:
    std::shared_ptr<DataBuffer_Impl> impl;
};

}
