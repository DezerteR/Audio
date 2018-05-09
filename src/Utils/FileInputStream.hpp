#pragma once

#include "InputStream.hpp"
#include <cstdio>
#include <string>

namespace Audio
{

class FileInputStream : public InputStream
{
public:

	FileInputStream();
	virtual ~FileInputStream();
	bool open(const std::string& filename);
	virtual i64 read(void* data, i64 size);
	virtual i64 seek(i64 position);
	virtual i64 tell();
	virtual i64 getSize();

private:
	std::FILE* m_file;
};

}
