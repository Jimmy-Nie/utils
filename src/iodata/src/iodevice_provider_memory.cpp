#include "iodevice_provider_memory.h"
#include <cstring>
#include <iostream>
namespace jm
{

IODeviceProvider_Memory::IODeviceProvider_Memory()
    : position(0)
{
}

IODeviceProvider_Memory::IODeviceProvider_Memory(int size)
    : data(DataBuffer(size))
{

}
IODeviceProvider_Memory::IODeviceProvider_Memory(DataBuffer &data)
    : data(data), position(0)
{
}

size_t IODeviceProvider_Memory::get_size() const
{
    return data.get_size();
}

size_t IODeviceProvider_Memory::get_position() const
{
    validate_position();
    return position;
}

const DataBuffer &IODeviceProvider_Memory::get_data() const
{
    return data;
}

DataBuffer &IODeviceProvider_Memory::get_data()
{
    return data;
}

size_t IODeviceProvider_Memory::send(const void *send_data, size_t len, bool send_all)
{
    validate_position();
    size_t size_needed = position + len;
    if (size_needed > data.get_size())
    {
        if (size_needed > data.get_capacity())	// Capacity exceeded
        {
            // Estimate the optimum databuffer capacity.
            data.set_capacity(size_needed * 1.5);
        }

        data.set_size(size_needed);
    }
    memcpy(data.get_data() + position, send_data, len);
    position += len;
    return len;
}

size_t IODeviceProvider_Memory::receive(void *recv_data, size_t len, bool receive_all)
{
    validate_position();
    size_t data_available = data.get_size() - position;
    if (len > data_available)
        len = data_available;
    memcpy(recv_data, data.get_data() + position, len);
    position += len;
    return len;
}

size_t IODeviceProvider_Memory::peek(void *recv_data, size_t len)
{
    validate_position();
    size_t data_available = data.get_size() - position;
    if (len > data_available)
        len = data_available;
    memcpy(recv_data, data.get_data() + position, len);
    return len;
}

bool IODeviceProvider_Memory::seek(int requested_position, IODevice::SeekMode mode)
{
    validate_position();
    int new_position = int(position);
    switch (mode)
    {
    case IODevice::seek_set:
        new_position = requested_position;
        break;
    case IODevice::seek_cur:
        new_position += requested_position;
        break;
    case IODevice::seek_end:
        new_position = int(data.get_size()) + requested_position;
        break;
    default:
        return false;
    }

    if (new_position >= 0 && new_position <= int(data.get_size()))
    {
        position = size_t(new_position);
        return true;
    }
    else
    {
        return false;
    }
}

IODeviceProvider *IODeviceProvider_Memory::duplicate()
{
    return new IODeviceProvider_Memory(data);
}

void IODeviceProvider_Memory::validate_position() const
{
    if (position > data.get_size())
        position = data.get_size();
}

}
