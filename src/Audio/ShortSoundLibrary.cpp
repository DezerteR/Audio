#include "ShortSoundLibrary.hpp"
#include "SoundBuffer.hpp"
#include "Utils.hpp"
#include <iostream>

namespace Audio
{

ShortSoundLibrary::ShortSoundLibrary(){}
ShortSoundLibrary::~ShortSoundLibrary(){}
void ShortSoundLibrary::loadDirectory(const std::string& directory){
	auto files = getFiles(directory, "*.vaw");
	for(auto &it : files){
        auto buffer = std::make_shared<SoundBuffer>();
		m_loadedClips.emplace(it.fileName, buffer);
		buffer->m_forceMono = true;
		buffer->loadFromFile(it.localFilePath);
        std::cout<<"- " << it.fileName<<"\n";
	}
}
SoundSource& ShortSoundLibrary::spawn(const std::string& clip){
    auto& buffer = m_loadedClips.at(clip);

    unsigned int source;
    alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer->m_buffer);
	m_activeSurces.emplace_back(source, buffer->getDuration().asMilliseconds());

    return m_activeSurces.back();
}
void ShortSoundLibrary::update(int microseconds){}
void ShortSoundLibrary::defaultVolume(float){}

}
