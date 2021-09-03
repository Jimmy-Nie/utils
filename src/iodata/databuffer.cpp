
#include "databuffer.h"
#include <string.h>

namespace jm
{
	class DataBuffer_Impl
	{
	public:
		DataBuffer_Impl() : data(nullptr), size(0), allocated_size(0)
		{
		}

		~DataBuffer_Impl()
		{
			delete[] data;
		}

	public:
		char *data;
		size_t size;
		size_t allocated_size;
	};

	DataBuffer::DataBuffer()
		: impl(std::make_shared<DataBuffer_Impl>())
	{
	}

	DataBuffer::DataBuffer(size_t new_size)
		: impl(std::make_shared<DataBuffer_Impl>())
	{
		set_size(new_size);
	}

	DataBuffer::DataBuffer(const DataBuffer &copy)
		: impl(copy.impl)
	{
		// Explicitly declared constructor like operator =
	}

	DataBuffer::DataBuffer(const void *new_data, size_t new_size)
		: impl(std::make_shared<DataBuffer_Impl>())
	{
		set_size(new_size);
		memcpy(impl->data, new_data, new_size);
	}

	DataBuffer::DataBuffer(const DataBuffer &new_data, size_t pos, size_t size)
		: impl(std::make_shared<DataBuffer_Impl>())
	{
		set_size(size);
		memcpy(impl->data, new_data.get_data() + pos, size);
	}

	DataBuffer::~DataBuffer()
	{
	}

	char *DataBuffer::get_data()
	{
		return impl->data;
	}

	const char *DataBuffer::get_data() const
	{
		return impl->data;
	}

	size_t DataBuffer::get_size() const
	{
		return impl->size;
	}

	size_t DataBuffer::get_capacity() const
	{
		return impl->allocated_size;
	}

	char &DataBuffer::operator[](size_t i)
	{
		return impl->data[i];
	}

	const char &DataBuffer::operator[](size_t i) const
	{
		return impl->data[i];
	}

	DataBuffer &DataBuffer::operator =(const DataBuffer &copy)
	{
		impl = copy.impl;
		return *this;
	}

	void DataBuffer::set_size(size_t new_size)
	{
		if (new_size > impl->allocated_size)
		{
			char *old_data = impl->data;
			impl->data = new char[new_size];
			memcpy(impl->data, old_data, impl->size);
			delete[] old_data;
			memset(impl->data + impl->size, 0, new_size - impl->size);
			impl->size = new_size;
			impl->allocated_size = new_size;
		}
		else
		{
			impl->size = new_size;
		}
	}

	void DataBuffer::set_capacity(size_t new_capacity)
	{
		if (new_capacity > impl->allocated_size)
		{
			char *old_data = impl->data;
			impl->data = new char[new_capacity];
			memcpy(impl->data, old_data, impl->size);
			delete[] old_data;
			memset(impl->data + impl->size, 0, new_capacity - impl->size);
			impl->allocated_size = new_capacity;
		}
	}

	bool DataBuffer::is_null() const
	{
		return impl->size == 0;
	}
}
