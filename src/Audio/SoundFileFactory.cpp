#include "SoundFileFactory.hpp"
#include "SoundFileReaderFlac.hpp"
#include "SoundFileReaderOgg.hpp"
#include "SoundFileReaderWav.hpp"
#include "FileInputStream.hpp"
#include "MemoryInputStream.hpp"

namespace
{
	// Register all the built-in readers and writers if not already done
	void ensureDefaultReadersWritersRegistered(){
		static bool registered = false;
		if(!registered)
		{
			Audio::SoundFileFactory::registerReader<Audio::SoundFileReaderFlac>();
			Audio::SoundFileFactory::registerReader<Audio::SoundFileReaderOgg>();
			Audio::SoundFileFactory::registerReader<Audio::SoundFileReaderWav>();
			registered = true;
		}
	}
}

namespace Audio
{
SoundFileFactory::ReaderFactoryArray SoundFileFactory::s_readers;

SoundFileReader* SoundFileFactory::createReaderFromFilename(const std::string& filename){
	// Register the built-in readers/writers on first call
	ensureDefaultReadersWritersRegistered();

	// Wrap the input file into a file stream
	FileInputStream stream;
	if(!stream.open(filename)) {
		cerr<< "Failed to open sound file \"" << filename << "\" (couldn't open stream)" << std::endl;
		return nullptr;
	}

	// Test the filename in all the registered factories
	for(ReaderFactoryArray::const_iterator it = s_readers.begin(); it != s_readers.end(); ++it){
		stream.seek(0);
		if(it->check(stream))
			return it->create();
	}

	// No suitable reader found
	cerr<< "Failed to open sound file \"" << filename << "\" (format not supported)" << std::endl;
	return nullptr;
}

SoundFileReader* SoundFileFactory::createReaderFromMemory(const void* data, std::size_t sizeInBytes){
	// Register the built-in readers/writers on first call
	ensureDefaultReadersWritersRegistered();

	// Wrap the memory file into a file stream
	MemoryInputStream stream;
	stream.open(data, sizeInBytes);

	// Test the stream for all the registered factories
	for(ReaderFactoryArray::const_iterator it = s_readers.begin(); it != s_readers.end(); ++it){
		stream.seek(0);
		if(it->check(stream))
			return it->create();
	}

	// No suitable reader found
	cerr<< "Failed to open sound file from memory (format not supported)" << std::endl;
	return nullptr;
}

/* SoundFileReader* SoundFileFactory::createReaderFromStream(InputStream& stream){
	// Register the built-in readers/writers on first call
	ensureDefaultReadersWritersRegistered();

	// Test the stream for all the registered factories
	for(ReaderFactoryArray::const_iterator it = s_readers.begin(); it != s_readers.end(); ++it){
		stream.seek(0);
		if(it->check(stream))
			return it->create();
	}

	// No suitable reader found
	cerr<< "Failed to open sound file from stream (format not supported)" << std::endl;
	return nullptr;
}
 */
}
