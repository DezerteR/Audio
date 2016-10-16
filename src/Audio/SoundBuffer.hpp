#pragma once
#include <set>
#include "Includes.hpp"
#include "Time.hpp"

namespace Audio
{
class Sound;
class InputSoundFile;
class InputStream;

class SoundBuffer
{
public:

	SoundBuffer();
	SoundBuffer(const SoundBuffer& copy);
	~SoundBuffer();
	bool loadFromFile(const std::string& filename);
	bool loadFromMemory(const void* data, std::size_t sizeInBytes);
	bool loadFromStream(InputStream& stream);
	bool loadFromSamples(const i16* samples, u64 sampleCount, u32 channelCount, u32 sampleRate);
	bool saveToFile(const std::string& filename) const;
	const i16* getSamples() const;
	u64 getSampleCount() const;
	u32 getSampleRate() const;
	u32 getChannelCount() const;
	Time getDuration() const;
	SoundBuffer& operator =(const SoundBuffer& right);

	bool initialize(InputSoundFile& file);
	void convertToMono();
	bool update(u32 channelCount, u32 sampleRate);

	bool m_forceMono { false };
	u32 m_buffer;
	std::vector<i16> m_samples;
	Time m_duration;
};

}
