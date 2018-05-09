#include <AL/al.h>
#include "Utils.hpp"
#include "AudioPlayer.hpp"
#include "SoundStream.hpp"
#include "SoundBuffer.hpp"
#include "AudioPlayerUtils.hpp"
#include "Includes.hpp"
#include <atomic>

namespace Audio
{
/*
/// --------------------------- StreamedPlayer2D ---------------------------
StreamedPlayer2D::StreamedPlayer2D(): m_music(std::make_shared<Audio::SoundStream>()), m_status(Stopped){
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	RNG.seed(seed);
	stop();
	m_music->m_onTrackEndCallback = [this]{
		this->next();
	};

}
StreamedPlayer2D::~StreamedPlayer2D(){}

void StreamedPlayer2D::next(u32 incr){
	if(incr){
		m_current = glm::clamp(m_current + incr, 0u, (u32)m_playlist.size());
	}
	else {
		std::uniform_int_distribution <u32> distribution(0, m_playlist.size());
		m_current = distribution(RNG);
	}
	open(m_current);
	play();
}
void StreamedPlayer2D::open(u32 id){
	stop();
	if(!m_music->openFromFile(m_playlist[id])){
		cerr<<"[ Error ] no file: " + m_playlist[id]<<endl;
		stop();
		return;
	}
}
void StreamedPlayer2D::play(){
	// if(m_status == Stopped){
		// m_current = glm::clamp(m_current, 0u, (u32)m_playlist.size());
		// open(m_current);
	// }
	m_music->play();
	m_status = Playing;
}
void StreamedPlayer2D::play(u32 id){
	// m_current = glm::clamp(id, 0u, (u32)m_playlist.size());

	// id = glm::clamp(id, 0u, (u32)m_playlist.size());
	// open(id);

	// m_music->play();
	// m_status = Play;
}
void StreamedPlayer2D::pause(){
	m_music->pause();
	m_status = Paused;
}
void StreamedPlayer2D::stop(){
	m_status = Stopped;
	m_music->stop();
}
void StreamedPlayer2D::loop(bool l){}

void StreamedPlayer2D::loadFromDirectory(const std::string &directory){
	m_current = 0;
	auto &&files = getFiles(directory, "*.ogg");
	for(auto &it : files){
		m_playlist.push_back(it.localFilePath);
	}
	open(m_current);
}

StreamedPlayer2D& StreamedPlayer2D::volume(float v){
	alSourcef(m_music->m_source, AL_GAIN, v);
	m_volume = v;
	return *this;
}
StreamedPlayer2D& StreamedPlayer2D::pitch(float p){
	alSourcef(m_music->m_source, AL_PITCH, p);
	m_pitch = p;
	return *this;
}


/// --------------------------- StreamedPlayer3D ---------------------------
StreamedPlayer3D::StreamedPlayer3D(){
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	RNG.seed(seed);
	m_music->m_forceMono = true;
	m_music->m_onTrackEndCallback = [this]{
		this->next();
	};

}

StreamedPlayer3D& StreamedPlayer3D::volume(float v){
	alSourcef(m_music->m_source, AL_GAIN, v);
	m_volume = v;
	return *this;
}
StreamedPlayer3D& StreamedPlayer3D::pitch(float p){
	alSourcef(m_music->m_source, AL_PITCH, p);
	m_pitch = p;
	return *this;
}
StreamedPlayer3D& StreamedPlayer3D::position(const glm::vec4 &p){
	alSource3f(m_music->m_source, AL_POSITION, p.x, p.y, p.z);
	m_position = p;
	return *this;
}
StreamedPlayer3D& StreamedPlayer3D::velocity(const glm::vec4 &v){
	alSource3f(m_music->m_source, AL_VELOCITY, v.x, v.y, v.z);
	m_velocity = v;
	return *this;
}
StreamedPlayer3D& StreamedPlayer3D::attenuation(float a){
	alSourcef(m_music->m_source, AL_ROLLOFF_FACTOR, a);
	m_attenuation = a;
	return *this;
}


/// --------------------------- SoundSource ---------------------------


/// --------------------------- SoundPlayer2D ---------------------------


void SoundPlayer2D::loadFromDirectory(const std::string &directory){
	auto &&files = getFiles(directory, "*.ogg");
	for(auto &it : files){
		stereoSoundBuffers.emplace_back(make_shared<SoundBuffer>());
		stereoSoundBuffers.back()->loadFromFile(it.localFilePath);
	}
}
SoundSource& SoundPlayer2D::get(u32 id){
	auto &buffer = *stereoSoundBuffers[id];
	u32 source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer.m_buffer);
	soundSources.emplace_back(source);
	return soundSources.back();
}
// SoundSource& SoundPlayer2D::get(const std::string&){
	// auto &buffer = stereoSoundBuffers[id];
	// alGenSources(1, &m_source);
	// alSourcei(m_source, AL_BUFFER, buffer.m_buffer);
	// soundSources.push_back(m_source);
	// return source;
// }
void SoundPlayer2D::update(i32 dt){
	std::remove_if(soundSources.begin(), soundSources.end(), [dt](SoundSource &source){
		if(source.m_lifetime){
			if((source.m_lifetime -= dt) <= 0){
				source.loop(false);
			}
		}
		return source.status() == Stopped;
	});
}

/// --------------------------- SoundPlayer3D ---------------------------


void SoundPlayer3D::loadFromDirectory(const std::string &directory){
	auto &&files = getFiles(directory, "*.ogg");
	for(auto &it : files){
		monoSoundBuffers.emplace_back(make_shared<SoundBuffer>());
		monoSoundBuffers.back()->m_forceMono = true;
		monoSoundBuffers.back()->loadFromFile(it.localFilePath);
	}
}
SoundSource& SoundPlayer3D::get(u32 id){
	auto &buffer = *monoSoundBuffers[id];
	u32 source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer.m_buffer);
	soundSources.emplace_back(source);
	return soundSources.back();
}
// SoundSource& SoundPlayer3D::get(const std::string&){
	// auto &buffer = monoSoundBuffers[id];
	// u32 source = 0;
	// alGenSources(1, &source);
	// alSourcei(m_source, AL_BUFFER, buffer.m_buffer);
	// soundSources.emplace_back(source);
// 	return source;
// }
void SoundPlayer3D::update(i32 dt){
	std::remove_if(soundSources.begin(), soundSources.end(), [dt](SoundSource &source){
		if(source.m_lifetime){
			if((source.m_lifetime -= dt) <= 0){
				source.loop(false);
			}
		}
		return source.status() == Stopped;
	});
}

/// --------------------------- Listener ---------------------------
*/
ContextHandler::ContextHandler(){
	Audio::AudioUtils::init();
	std::cout<<"Audio context initialized"<<std::endl;
}
ContextHandler::~ContextHandler(){
	Audio::AudioUtils::clear();
	std::cout<<"Audio context deleted"<<std::endl;
}

}
