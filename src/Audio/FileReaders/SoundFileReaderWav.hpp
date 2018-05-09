#pragma once
#include "Includes.hpp"

#include "SoundFileReader.hpp"
#include <string>

namespace Audio
{
class SoundFileReaderWav : public SoundFileReader
{
public:

	static bool check(InputStream& stream);

public:

	SoundFileReaderWav();

	virtual bool open(Audio::InputStream& stream, Info& info);

	virtual void seek(u64 sampleOffset);

	virtual u64 read(i16* samples, u64 maxCount);

private:

	bool parseHeader(Info& info);

	// Member data

	InputStream* m_stream;		 /// Source stream to read from
	u32 m_bytesPerSample; /// Size of a sample, in bytes
	u64	   m_dataStart;	  /// Starting position of the audio data in the open file
};

}
