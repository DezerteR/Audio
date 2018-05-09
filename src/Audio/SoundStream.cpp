#include "SoundStreamImpl.hpp"
#include "SoundStream.hpp"

namespace Audio
{

SoundStream::SoundStream() : m_impl(std::make_unique<SoundStreamImpl>()){}
SoundStream::~SoundStream() = default;

SoundStream& SoundStream::volume(float v){
	alSourcef(m_impl->m_source, AL_GAIN, v);
	return *this;
}
SoundStream& SoundStream::pitch(float p){
	alSourcef(m_impl->m_source, AL_PITCH, p);
	return *this;
}
SoundStream& SoundStream::position(const glm::vec4& p){
	alSource3f(m_impl->m_source, AL_POSITION, p.x, p.y, p.z);
	return *this;
}
SoundStream& SoundStream::velocity(const glm::vec4& v){
	alSource3f(m_impl->m_source, AL_VELOCITY, v.x, v.y, v.z);
	return *this;
}
SoundStream& SoundStream::direction(const glm::vec4& d){
	alSource3f(m_impl->m_source, AL_DIRECTION, d.x, d.y, d.z);
	return *this;
}
SoundStream& SoundStream::cone(float inner, float outer){
	alSourcef(m_impl->m_source, AL_CONE_INNER_ANGLE, inner);
	alSourcef(m_impl->m_source, AL_CONE_OUTER_ANGLE, outer);
	return *this;
}
SoundStream& SoundStream::attenuation(float a){
	alSourcef(m_impl->m_source, AL_ROLLOFF_FACTOR, a);
	return *this;
}
SoundStream& SoundStream::relativeToListener(bool r){
	alSourcei(m_impl->m_source, AL_SOURCE_RELATIVE, r);
	return *this;
}
SoundStream& SoundStream::loop(bool l){
	m_impl->setLoop(l);
	return *this;
}
SoundStream& SoundStream::onStreamFinish(std::function<void(void)>&& fun){
	m_impl->m_onStreamFinish = fun;
	return *this;
}

bool SoundStream::openFromFile(const std::string& filename){
    return m_impl->openFromFile(filename);
}

bool SoundStream::openFromMemory(const void* data, std::size_t sizeInBytes){
    return m_impl->openFromMemory(data, sizeInBytes);
}
}
