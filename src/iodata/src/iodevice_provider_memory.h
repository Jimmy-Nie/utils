#pragma once

#include "iodata/iodevice_provider.h"
#include "iodata/databuffer.h"

namespace jm
{

class IODeviceProvider_Memory : public IODeviceProvider
{
public:
    IODeviceProvider_Memory();
    IODeviceProvider_Memory(int size);
    IODeviceProvider_Memory(DataBuffer &data);

    virtual size_t get_size() const override;
    virtual size_t get_position() const override;

    const DataBuffer &get_data() const;
    DataBuffer &get_data();

    virtual size_t send(const void *data, size_t len, bool send_all = true) override;
    virtual size_t receive(void *data, size_t len, bool receive_all = true) override;
    virtual size_t peek(void *data, size_t len) override;
    virtual bool seek(int position, IODevice::SeekMode mode) override;
    IODeviceProvider *duplicate() override;

private:
    void validate_position() const;

    DataBuffer data;
    mutable size_t position;
};
}
