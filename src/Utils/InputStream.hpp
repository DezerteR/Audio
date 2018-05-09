#pragma once
#include "Includes.hpp"

namespace Audio
{

class InputStream
{
public:

	virtual ~InputStream() {}

	virtual i64 read(void* data, i64 size) = 0;

	virtual i64 seek(i64 position) = 0;

	virtual i64 tell() = 0;

	virtual i64 getSize() = 0;
};

}

