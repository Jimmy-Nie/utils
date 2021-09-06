

#pragma once

#include "iodata/iodevice.h"

namespace jm
{

class DataBuffer;

/// \brief Memory I/O device.
class MemoryDevice : public IODevice
{
public:
    /// \brief Constructs a memory I/O device.
    MemoryDevice();

    /// \brief Constructs a IODevice Memory
    ///
    /// \param size = Data Buffer size
    MemoryDevice(int size);

    /// \brief Constructs a IODevice Memory
    ///
    /// \param data = Data Buffer
    MemoryDevice(DataBuffer &data);

    /// \brief Retrieves the data buffer for the memory device.
    const DataBuffer &get_data() const;

    /// \brief Get Data
    ///
    /// \return data
    DataBuffer &get_data();
};

}
