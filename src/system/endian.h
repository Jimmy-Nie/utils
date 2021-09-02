#pragma once

#include <cstdint>

namespace jm
{

/// \addtogroup bvcore_I_O_Data bvcore I/O Data
/// \{

/// \brief Endianess management class.
class Endian
{
private:
    /// \brief Swaps larger amounts of data between little and big endian.
    ///
    /// \param data Data to be swapped.
    /// \param type_size Size of datatype to be swapped. (Must be an even number)
    /// \param total_times Number of 'type_size' size data chunks to be swapped.
    static void swap(void *data, int type_size, int total_times = 1) {
        if (type_size == 1) return;
        auto *d = (uint8_t *)data;
        for (int j = 0; j < total_times; j++) {
            for (int i = 0; i < type_size/2; i++) {
                unsigned char a = d[i];
                d[i] = d[type_size - 1 - i];
                d[type_size - 1 - i] = a;
            }
            d += type_size;
        }
    }

public:
    /// \brief: swap the data to little/big endian
    static void swap_to_little_endian(void *data, int type_size, int total_times = 1) {
        if (is_system_big())
            swap(data, type_size, total_times);
    }
    static void swap_to_big_endian(void *data, int type_size, int total_times = 1) {
        if (!is_system_big())
            swap(data, type_size, total_times);
    }

    /// \brief Returns true if big endian system.
    /** \return true if big endian system, false otherwise.*/
    static bool is_system_big() {
        const int i = 1;
        return !(*(char*)&i);
    }

    /// \brief Returns true if 64 bit system.
    /** \return True if 64 bit, false otherwise.*/
    static bool is_system_64bit() {
        return (sizeof(int*) == 8);
    }
};

}
