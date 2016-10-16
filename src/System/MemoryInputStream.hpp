#pragma once
#include "System/InputStream.hpp"
#include <cstdlib>

namespace Audio
{

class MemoryInputStream : public InputStream
{
public:
	MemoryInputStream();

	void open(const void* data, std::size_t sizeInBytes);
	virtual i64 read(void* data, i64 size);
	virtual i64 seek(i64 position);
	virtual i64 tell();
	virtual i64 getSize();

private:
	const char* m_data;
	i64	   m_size;
	i64	   m_offset;
};

}

