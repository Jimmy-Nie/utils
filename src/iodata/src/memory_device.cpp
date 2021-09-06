#include "iodata/memory_device.h"
#include "iodevice_provider_memory.h"

namespace jm
{
MemoryDevice::MemoryDevice()
    : IODevice(new IODeviceProvider_Memory())
{
}

MemoryDevice::MemoryDevice(int size)
    : IODevice(new IODeviceProvider_Memory(size))
{

}
MemoryDevice::MemoryDevice(DataBuffer &data)
    : IODevice(new IODeviceProvider_Memory(data))
{
}

const DataBuffer &MemoryDevice::get_data() const
{
    const auto *provider = dynamic_cast<const IODeviceProvider_Memory*>(get_provider());
    return provider->get_data();
}

DataBuffer &MemoryDevice::get_data()
{
    auto *provider = dynamic_cast<IODeviceProvider_Memory*>(get_provider());
    return provider->get_data();
}
}
