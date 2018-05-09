#pragma once
#include <FLAC/stream_decoder.h>
#include <string>
#include <vector>
#include "Includes.hpp"
#include "SoundFileReader.hpp"

namespace Audio
{
	
class SoundFileReaderFlac : public SoundFileReader
{
public:

	static bool check(InputStream& stream);

public:

	SoundFileReaderFlac();

	~SoundFileReaderFlac();

	virtual bool open(InputStream& stream, Info& info);

	virtual void seek(u64 sampleOffset);

	virtual u64 read(i16* samples, u64 maxCount);

public:

	struct ClientData
	{
		InputStream* stream;
		SoundFileReader::Info info;
		i16* buffer;
		u64 remaining;
		std::vector<i16> leftovers;
		bool error;
	};

private:

	void close();

	// Member data

	FLAC__StreamDecoder* m_decoder;
	ClientData m_clientData;
};

}
