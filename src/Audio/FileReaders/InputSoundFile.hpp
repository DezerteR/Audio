#pragma once
#include "Includes.hpp"
#include "Time.hpp"
#include <string>

namespace Audio
{
class InputStream;
class SoundFileReader;

class InputSoundFile
{
public:
	InputSoundFile();
	~InputSoundFile();
	bool openFromFile(const std::string& filename);
	bool openFromMemory(const void* data, std::size_t sizeInBytes);
	// bool openFromStream(InputStream& stream);
	bool openForWriting(const std::string& filename, u32 channelCount, u32 sampleRate);
	u64 getSampleCount() const;
	u32 getChannelCount() const;
	u32 getSampleRate() const;
	Time getDuration() const;
	void seek(u64 sampleOffset);
	void seek(Time timeOffset);
	u64 read(i16* samples, u64 maxCount);
	bool m_forceMono;

private:

	void close();

	SoundFileReader* m_reader; /// Reader that handles I/O on the file's format
	InputStream* m_stream; /// Input stream used to access the file's data
	bool m_streamOwned; /// Is the stream internal or external?
	u64 m_sampleCount; /// Total number of samples in the file
	u32 m_channelCount; /// Number of channels of the sound
	u32 m_sampleRate; /// Number of samples per second
};

}
