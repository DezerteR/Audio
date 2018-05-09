#include "SoundSource.hpp"

namespace Audio
{

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
SoundSource& SoundSource::lifetime(int time){
    m_lifetime = std::max(m_msLenght, time);
    if(time) loop();
    return *this;
}
SoundSource& SoundSource::offset(int time){
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
SoundSource& SoundSource::play(){
    alSourcePlay(m_source);
    return *this;
}

int SoundSource::miliseconds() const {
    return m_msLenght;
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
}
