#include <AL/al.h>
#include "Utils.hpp"
#include "AudioPlayer.hpp"
#include "StreamedSoundSource.hpp"
#include "SoundBuffer.hpp"
#include "AudioPlayerUtils.hpp"
#include "Includes.hpp"
#include <atomic>

namespace Audio
{

/// --------------------------- StreamedPlayer2D ---------------------------
StreamedPlayer2D::StreamedPlayer2D(u32 id) : id(id), m_music(std::make_unique<Audio::StreamedSoundSource>()), m_status(Stopped){
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	RNG.seed(seed);

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
	open(0);
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
StreamedPlayer3D::StreamedPlayer3D(u32 id) : StreamedPlayer2D(id){
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
std::vector<SoundSource> g_soundSources;

SoundSource& SoundSource::volume(float v){
	alSourcef(m_source, AL_GAIN, v);
	return *this;
}
SoundSource& SoundSource::pitch(float p){
	alSourcef(m_source, AL_PITCH, p);
	return *this;
}
SoundSource& SoundSource::position(const glm::vec4 &p){
	alSource3f(m_source, AL_POSITION, p.x, p.y, p.z);
	return *this;
}
SoundSource& SoundSource::velocity(const glm::vec4 &v){
	alSource3f(m_source, AL_VELOCITY, v.x, v.y, v.z);
	return *this;
}
SoundSource& SoundSource::direction(const glm::vec4 &d){
	alSource3f(m_source, AL_DIRECTION, d.x, d.y, d.z);
	return *this;
}
SoundSource& SoundSource::cone(float inner, float outer){
	alSourcef(m_source, AL_CONE_INNER_ANGLE, inner);
	alSourcef(m_source, AL_CONE_OUTER_ANGLE, outer);
	return *this;
}
SoundSource& SoundSource::attenuation(float a){
	alSourcef(m_source, AL_ROLLOFF_FACTOR, a);
	return *this;
}
SoundSource& SoundSource::relativeToListener(bool r){
	alSourcei(m_source, AL_SOURCE_RELATIVE, r);
	return *this;
}
SoundSource& SoundSource::loop(bool l){
	alSourcei(m_source, AL_LOOPING, l);
	return *this;
}
SoundSource& SoundSource::lifetime(i32 time){
	m_lifetime = time;
	if(time) loop();
	return *this;
}
SoundSource& SoundSource::offset(i32 time){
	alSourcef(m_source, AL_SEC_OFFSET, time/1000.f);
	return *this;
}
Status SoundSource::status() const {
	ALint status;
	alGetSourcei(m_source, AL_SOURCE_STATE, &status);

	switch (status){
		case AL_INITIAL:
		case AL_STOPPED: return Stopped;
		case AL_PAUSED:  return Paused;
		case AL_PLAYING: return Playing;
	}

	return Stopped;
}
void SoundSource::play(){
	alSourcePlay(m_source);
}
void SoundSource::update(i32 dt){
	for(auto it = g_soundSources.begin(); it != g_soundSources.end(); it++){
		if(it->m_lifetime){
			if((it->m_lifetime -= dt) <= 0){
				it->loop(false);
			}
		}
		if(it->status() == Stopped){
			it = g_soundSources.erase(it);
		}
	}
}

glm::vec4 SoundSource::getPosition() const {
	glm::vec4 p;
	alGetSource3f(m_source, AL_POSITION, &p.x, &p.y, &p.z);
	p.w = 1.f;
	return p;
}
glm::vec4 SoundSource::getVelocity() const {
	glm::vec4 v;
	alGetSource3f(m_source, AL_VELOCITY, &v.x, &v.y, &v.z);
	v.w = 0.f;
	return v;
}
glm::vec4 SoundSource::getDirection() const {
	glm::vec4 d;
	alGetSource3f(m_source, AL_DIRECTION, &d.x, &d.y, &d.z);
	d.w = 0.f;
	return d;
}

float SoundSource::getInnerCone() const {
	ALfloat inner;
	alGetSourcef(m_source, AL_CONE_INNER_ANGLE, &inner);

	return inner;
}
float SoundSource::getOuterCone() const {
	ALfloat outer;
	alGetSourcef(m_source, AL_CONE_OUTER_ANGLE, &outer);

	return outer;
}
float SoundSource::getPitch() const {
	ALfloat pitch;
	alGetSourcef(m_source, AL_PITCH, &pitch);

	return pitch;
}
float SoundSource::getVolume() const {
	ALfloat gain;
	alGetSourcef(m_source, AL_GAIN, &gain);

	return gain * 100.f;
}
float SoundSource::getMinDistance() const {
	ALfloat distance;
	alGetSourcef(m_source, AL_REFERENCE_DISTANCE, &distance);

	return distance;
}
float SoundSource::getAttenuation() const {
	ALfloat attenuation;
	alGetSourcef(m_source, AL_ROLLOFF_FACTOR, &attenuation);

	return attenuation;
}

bool SoundSource::isRelativeToListener() const {
	ALint relative;
	alGetSourcei(m_source, AL_SOURCE_RELATIVE, &relative);

	return relative != 0;
}


/// --------------------------- SoundPlayer2D ---------------------------
std::vector<Audio::SoundBuffer> g_stereoSoundBuffers;

void SoundPlayer2D::loadFromDirectory(const std::string &directory){
	auto &&files = getFiles(directory, "*.ogg");
	for(auto &it : files){
		Audio::SoundBuffer buffer;
		buffer.loadFromFile(it.localFilePath);
		g_stereoSoundBuffers.push_back(buffer);
	}
}
SoundSource& SoundPlayer2D::get(u32 id){
	auto &buffer = g_stereoSoundBuffers[id];
	u32 source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer.m_buffer);
	g_soundSources.emplace_back(source);
	return g_soundSources.back();
}
// SoundSource& SoundPlayer2D::get(const std::string&){
	// auto &buffer = g_stereoSoundBuffers[id];
	// alGenSources(1, &m_source);
	// alSourcei(m_source, AL_BUFFER, buffer.m_buffer);
	// g_soundSources.push_back(m_source);
	// return source;
// }

/// --------------------------- SoundPlayer3D ---------------------------
std::vector<Audio::SoundBuffer> g_monoSoundBuffers;

void SoundPlayer3D::loadFromDirectory(const std::string &directory){
	auto &&files = getFiles(directory, "*.ogg");
	for(auto &it : files){
		Audio::SoundBuffer buffer;
		buffer.m_forceMono = true;
		buffer.loadFromFile(it.localFilePath);
		g_monoSoundBuffers.push_back(buffer);
	}
}
SoundSource& SoundPlayer3D::get(u32 id){
	auto &buffer = g_monoSoundBuffers[id];
	u32 source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer.m_buffer);
	g_soundSources.emplace_back(source);
	return g_soundSources.back();
}
// SoundSource& SoundPlayer3D::get(const std::string&){
	// auto &buffer = g_monoSoundBuffers[id];
	// u32 source = 0;
	// alGenSources(1, &source);
	// alSourcei(m_source, AL_BUFFER, buffer.m_buffer);
	// g_soundSources.emplace_back(source);
// 	return source;
// }


/// --------------------------- Listener ---------------------------
void Listener::position(const glm::vec4 &p){
	alListener3f(AL_POSITION, p.x, p.y, p.z);
}
void Listener::velocity(const glm::vec4 &v){
	alListener3f(AL_VELOCITY, v.x, v.y, v.z);
}
void Listener::orientation(const glm::mat4 &view){
	orientation(view * glm::vec4(0,0,1,0), view * glm::vec4(0,1,0,0));
}
void Listener::orientation(const glm::vec4 &z, const glm::vec4 &y){
	m_at = z;
	m_up = y;
	float orientation[] = {m_at.x, m_at.y, m_at.z,
						   m_up.x, m_up.y, m_up.z};

	alListenerfv(AL_ORIENTATION, orientation);
}
/// at i up muszą być ortogonalne, trzeba odciąć część wektora
/// choć ta funkcja nie jest potrzebna
void Listener::up(const glm::vec4 &u){
	m_up = u;
}
void Listener::at(const glm::vec4 &a){
	m_at = a;
}
void Listener::volume(float v){
	alListenerf(AL_GAIN, v);
	m_volume = v;
}

float Listener::m_volume(1);
glm::vec4 Listener::m_at(1,0,0,0);
glm::vec4 Listener::m_up(0,0,1,0);

ContextHandler::ContextHandler(){
	Audio::AudioUtils::init();
}
ContextHandler::~ContextHandler(){
	Audio::AudioUtils::clear();
}

}
