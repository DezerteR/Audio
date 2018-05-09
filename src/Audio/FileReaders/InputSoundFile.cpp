
#include "InputSoundFile.hpp"
#include "SoundFileReader.hpp"
#include "SoundFileFactory.hpp"
#include "InputStream.hpp"
#include "FileInputStream.hpp"
#include "MemoryInputStream.hpp"
#include "AudioPlayerUtils.hpp"

namespace Audio
{
InputSoundFile::InputSoundFile() :
m_reader(NULL),
m_stream(NULL),
m_streamOwned(false),
m_sampleCount(0),
m_channelCount(0),
m_forceMono(false),
m_sampleRate(0){
}

InputSoundFile::~InputSoundFile(){
	// Close the file in case it was open
	close();
}

bool InputSoundFile::openFromFile(const std::string& filename){
	// If the file is already open, first close it
	close();

	// Find a suitable reader for the file type
	m_reader = SoundFileFactory::createReaderFromFilename(filename);
	if(!m_reader)
		return false;

	// Wrap the file into a stream
	FileInputStream* file = new FileInputStream;
	m_stream = file;
	m_streamOwned = true;

	// Open it
	if(!file->open(filename)){
		close();
		return false;
	}

	// Pass the stream to the reader
	SoundFileReader::Info info;
	if(!m_reader->open(*file, info)){
		close();
		return false;
	}

	// Retrieve the attributes of the open sound file
	m_sampleCount = info.sampleCount;
	m_channelCount = info.channelCount;
	if(m_forceMono){
		if(m_channelCount == 2){
			m_channelCount = 1;
			m_sampleCount /= 2;
		}
		else {
			m_forceMono = false;
		}
	}
	m_sampleRate = info.sampleRate;

	return true;
}

bool InputSoundFile::openFromMemory(const void* data, std::size_t sizeInBytes){
	// If the file is already open, first close it
	close();

	// Find a suitable reader for the file type
	m_reader = SoundFileFactory::createReaderFromMemory(data, sizeInBytes);
	if(!m_reader)
		return false;

	// Wrap the memory file into a stream
	MemoryInputStream* memory = new MemoryInputStream;
	m_stream = memory;
	m_streamOwned = true;

	// Open it
	memory->open(data, sizeInBytes);

	// Pass the stream to the reader
	SoundFileReader::Info info;
	if(!m_reader->open(*memory, info)){
		close();
		return false;
	}

	// Retrieve the attributes of the open sound file
	m_sampleCount = info.sampleCount;
	m_channelCount = info.channelCount;
	if(m_forceMono){
		if(m_channelCount == 2){
			m_channelCount = 1;
			m_sampleCount /= 2;
		}
		else {
			m_forceMono = false;
		}
	}
	m_sampleRate = info.sampleRate;

	return true;
}

/*bool InputSoundFile::openFromStream(InputStream& stream){
	// If the file is already open, first close it
	close();

	// Find a suitable reader for the file type
	m_reader = SoundFileFactory::createReaderFromStream(stream);
	if(!m_reader)
		return false;

	// store the stream
	m_stream = &stream;
	m_streamOwned = false;

	// Don't forget to reset the stream to its beginning before re-opening it
	if(stream.seek(0) != 0){
		cerr<< "Failed to open sound file from stream (cannot restart stream)" << std::endl;
		return false;
	}

	// Pass the stream to the reader
	SoundFileReader::Info info;
	if(!m_reader->open(stream, info)){
		close();
		return false;
	}

	// Retrieve the attributes of the open sound file
	m_sampleCount = info.sampleCount;
	m_channelCount = info.channelCount;
	m_sampleRate = info.sampleRate;

	return true;
} */

u64 InputSoundFile::getSampleCount() const {
	return m_sampleCount;
}

u32 InputSoundFile::getChannelCount() const {
	return m_channelCount;
}

u32 InputSoundFile::getSampleRate() const {
	return m_sampleRate;
}

Time InputSoundFile::getDuration() const {
	return seconds(static_cast<float>(m_sampleCount) / m_channelCount / m_sampleRate);
}

void InputSoundFile::seek(u64 sampleOffset){
	if(m_reader)
		m_reader->seek(sampleOffset);
}

void InputSoundFile::seek(Time timeOffset){
	seek(static_cast<u64>(timeOffset.asSeconds() * m_sampleRate * m_channelCount));
}

u64 InputSoundFile::read(i16* samples, u64 maxCount){
	// cout<<maxCount<<endl;
	if(m_reader && samples && maxCount){
		// u64 count = 0;
		// if(m_forceMono){
			// std::vector<i16> data(maxCount*2);
			// count = m_reader->read(samples, maxCount);
			// count = AudioUtils::convertToMono(samples, count);
		// }
		// else {
			// u64 count = m_reader->read(samples, maxCount);
		// }
		// return count;
		return m_reader->read(samples, maxCount);
	}
	else
		return 0;
}

void InputSoundFile::close(){
	// Destroy the reader
	delete m_reader;
	m_reader = NULL;

	// Destroy the stream if we own it
	if(m_streamOwned){
		delete m_stream;
		m_streamOwned = false;
	}
	m_stream = NULL;

	// Reset the sound file attributes
	m_sampleCount = 0;
	m_channelCount = 0;
	m_sampleRate = 0;
}

}
