#pragma once
#include "Includes.hpp"
#include <string>

namespace Audio
{

class InputStream;

class SoundFileReader
{
public:

	struct Info
	{
		u64 sampleCount;
		u32 channelCount;
		u32 sampleRate;
	};

	virtual ~SoundFileReader() {}

	virtual bool open(InputStream& stream, Info& info) = 0;

	virtual void seek(u64 sampleOffset) = 0;

	virtual u64 read(i16* samples, u64 maxCount) = 0;
};

}
