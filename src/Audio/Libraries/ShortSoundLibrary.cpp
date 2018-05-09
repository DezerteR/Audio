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
		buffer->m_forceMono = m_forceMono;
		buffer->loadFromFile(it.localFilePath);
        m_loadedClipsLinear.push_back(buffer);
        std::cout<<"- " << it.fileName<<"\n";
	}
}
SoundSource& ShortSoundLibrary::spawn(const std::string& clip){
    auto& buffer = m_loadedClips.at(clip);

    unsigned int source;
    alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer->m_buffer);
	m_activeSurces.emplace_back(source, buffer->getDuration().asMilliseconds());

    return m_activeSurces.back().volume(m_volume);
}
void ShortSoundLibrary::update(int microseconds){}
void ShortSoundLibrary::defaultVolume(float v){
    m_volume = v;
}
void ShortSoundLibrary::mono(bool m){
    m_forceMono = m;
}

}
