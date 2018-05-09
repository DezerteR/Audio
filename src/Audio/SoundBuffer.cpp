#include "SoundBuffer.hpp"
#include "InputSoundFile.hpp"
#include "AudioPlayerUtils.hpp"
#include <memory>

namespace Audio
{

SoundBuffer::SoundBuffer() :
m_buffer  (0),
m_duration(){
	// Create the buffer
	alGenBuffers(1, &m_buffer);
}

SoundBuffer::SoundBuffer(const SoundBuffer& copy) :
m_buffer  (0),
m_samples (copy.m_samples),
m_duration(copy.m_duration)
{
	// Create the buffer
	alGenBuffers(1, &m_buffer);

	// Update the internal buffer with the new samples
	update(copy.getChannelCount(), copy.getSampleRate());
}

SoundBuffer::~SoundBuffer(){
	if(m_buffer)
		alDeleteBuffers(1, &m_buffer);
}

bool SoundBuffer::loadFromFile(const std::string& filename){
	InputSoundFile file;
	if(file.openFromFile(filename))
		return initialize(file);
	else
		return false;
}

bool SoundBuffer::loadFromMemory(const void* data, std::size_t sizeInBytes){
	InputSoundFile file;
	if(file.openFromMemory(data, sizeInBytes))
		return initialize(file);
	else
		return false;
}

bool SoundBuffer::loadFromStream(InputStream& stream){
	InputSoundFile file;
	// if(file.openFromStream(stream))
		// return initialize(file);
	// else
		return false;
}

bool SoundBuffer::loadFromSamples(const i16* samples, u64 sampleCount, u32 channelCount, u32 sampleRate){
	if(samples && sampleCount && channelCount && sampleRate){
		m_samples.assign(samples, samples + sampleCount);

		return update(channelCount, sampleRate);
	}
	else {
		// Error...
		std::cerr << "Failed to load sound buffer from samples ("
			  << "array: "	  << samples	  << ", "
			  << "count: "	  << sampleCount  << ", "
			  << "channels: "   << channelCount << ", "
			  << "samplerate: " << sampleRate   << ")"
			  << std::endl;

		return false;
	}
}

const i16* SoundBuffer::getSamples() const {
	return m_samples.empty() ? nullptr : &m_samples[0];
}

u64 SoundBuffer::getSampleCount() const {
	return m_samples.size();
}

u32 SoundBuffer::getSampleRate() const {
	ALint sampleRate;
	alGetBufferi(m_buffer, AL_FREQUENCY, &sampleRate);

	return sampleRate;
}

u32 SoundBuffer::getChannelCount() const {
	ALint channelCount;
	alGetBufferi(m_buffer, AL_CHANNELS, &channelCount);

	return channelCount;
}

Time SoundBuffer::getDuration() const {
	return m_duration;
}

SoundBuffer& SoundBuffer::operator = (const SoundBuffer& right){
	SoundBuffer temp(right);

	std::swap(m_samples,  temp.m_samples);
	std::swap(m_buffer,   temp.m_buffer);
	std::swap(m_duration, temp.m_duration);

	return *this;
}

bool SoundBuffer::initialize(InputSoundFile& file){
	u64 sampleCount  = file.getSampleCount();
	u32 channelCount = file.getChannelCount();
	u32 sampleRate   = file.getSampleRate();

	// Read the samples from the provided file
	m_samples.resize(static_cast<std::size_t>(sampleCount));
	if(file.read(&m_samples[0], sampleCount) == sampleCount){
		return update(channelCount, sampleRate);
	}
	else{
		return false;
	}
}

bool SoundBuffer::update(u32 channelCount, u32 sampleRate){
	if(!channelCount || !sampleRate || m_samples.empty())
		return false;

	if(m_forceMono && channelCount == 2){
		AudioUtils::convertToMono(m_samples);
		channelCount = 1;
	}

	ALenum format = AudioUtils::calcFormatFromChannelCount(channelCount);

	if(format == 0){
		cerr<< "Failed to load sound buffer (unsupported number of channels: " << channelCount << ")" << std::endl;
		return false;
	}

	// Fill the buffer
	ALsizei binarySize = static_cast<ALsizei>(m_samples.size()) * sizeof(i16);
	alBufferData(m_buffer, format, m_samples.data(), binarySize, sampleRate);

	m_duration = seconds(static_cast<float>(m_samples.size()) / sampleRate / channelCount);

	return true;
}

}
