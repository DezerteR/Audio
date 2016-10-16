#pragma once
#include "Includes.hpp"

#include "SoundFileReader.hpp"
#include <vorbis/vorbisfile.h>

namespace Audio
{
class SoundFileReaderOgg : public SoundFileReader
{
public:

	static bool check(InputStream& stream);

public:

	SoundFileReaderOgg();

	~SoundFileReaderOgg();

	virtual bool open(InputStream& stream, Info& info);

	virtual void seek(u64 sampleOffset);

	virtual u64 read(i16* samples, u64 maxCount);

private:

	void close();

	// Member data

	OggVorbis_File m_vorbis;	   // ogg/vorbis file handle
	u32   m_channelCount; // number of channels of the open sound file
};

}
