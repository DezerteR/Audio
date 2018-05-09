#pragma once
#include "AudioPlayerUtils.hpp"

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_RADIANS
#include <vector>
#include <glm/glm.hpp>

namespace Audio
{

class SoundSource
{
public:
    SoundSource() : m_source(0), m_lifetime(0){}
    SoundSource(u32 id, int msLenght) : m_source(id), m_msLenght(msLenght){}
    SoundSource& play();

    /// setters
    /// volume is in [0,1]
    SoundSource& volume(float);
    SoundSource& pitch(float);
    SoundSource& position(const glm::vec4&);
    SoundSource& velocity(const glm::vec4&);
    SoundSource& direction(const glm::vec4&);
    SoundSource& cone(float inner, float outer = 360.f);
    SoundSource& attenuation(float);
    SoundSource& relativeToListener(bool r = true);
    SoundSource& loop(bool l = true);
    /// sets minimal lifetime, sound is looped, and when
    /// time ends, loop isremoved, time is in miliseconsds[ms]
    SoundSource& lifetime(int);
    /// offset to sound start time, in miliseconsds[ms]
    SoundSource& offset(int);
    Status status() const;

    /// getters
    glm::vec4 getPosition() const;
    glm::vec4 getVelocity() const;
    glm::vec4 getDirection() const;

    int miliseconds() const;

    float getInnerCone() const;
    float getOuterCone() const;
    float getPitch() const;
    float getVolume() const;
    float getAttenuation() const;
    float getMinDistance() const;
    bool getLoop() const;
    bool isRelativeToListener() const;

    int m_lifetime {};
    int m_msLenght {};
    unsigned int m_source;
};

}
