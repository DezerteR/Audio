#pragma once
#include "Includes.hpp"

#include <string>
#include <vector>

namespace Audio
{

class InputStream;
class SoundFileReader;
class SoundFileWriter;

class SoundFileFactory
{
public:

	template <typename T>
	static void registerReader();

	template <typename T>
	static void unregisterReader();
	static SoundFileReader* createReaderFromFilename(const std::string& filename);

	static SoundFileReader* createReaderFromMemory(const void* data, std::size_t sizeInBytes);

	// static SoundFileReader* createReaderFromStream(InputStream& stream);
private:

	// Types

	struct ReaderFactory
	{
		bool (*check)(InputStream&);
		SoundFileReader* (*create)();
	};
	typedef std::vector<ReaderFactory> ReaderFactoryArray;
	// Static member data

	static ReaderFactoryArray s_readers; /// List of all registered readers
};

}

#include <SoundFileFactory.inl>
