
#pragma once

#include "iodata/iodevice.h"

namespace jm
{

/// \brief I/O device provider interface.
class IODeviceProvider
{
public:
virtual ~IODeviceProvider() { }

/// \brief Returns the size of data stream.
/** <p>Returns SIZE_MAX if the size is unknown.</p>*/
virtual size_t get_size() const { return size_t(SIZE_MAX); }

/// \brief Returns the position in the data stream.
/** <p>Returns SIZE_MAX if the position is unknown.</p>*/
virtual size_t get_position() const { return size_t(SIZE_MAX); }

/// \brief Send data to device.
virtual size_t send(const void *data, size_t len, bool send_all = true) = 0;

/// \brief Receive data from device.
virtual size_t receive(void *data, size_t len, bool receive_all = true) = 0;

/// \brief Peek data from device.
virtual size_t peek(void *data, size_t len) = 0;

/// \brief Returns a new provider to the same resource.
virtual IODeviceProvider *duplicate() = 0;

/// \brief Seek in data stream.
virtual bool seek(int /*position*/, IODevice::SeekMode /*mode*/) { return false; }
};

}
