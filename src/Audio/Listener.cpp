#include <AL/al.h>
#include "Utils.hpp"
#include "SoundStream.hpp"
#include "SoundBuffer.hpp"
#include "AudioPlayerUtils.hpp"
#include "Includes.hpp"
#include "Listener.hpp"
#include <atomic>

namespace Audio
{

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
	float mat[] = {m_at.x, m_at.y, m_at.z,
						   m_up.x, m_up.y, m_up.z};

	alListenerfv(AL_ORIENTATION, mat);
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
}
