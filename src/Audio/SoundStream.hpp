#pragma once
#include <memory>
#include <functional>

namespace Audio
{

class SoundStreamImpl;

class SoundStream
{
private:
    std::unique_ptr<SoundStreamImpl> m_impl;
public:
    SoundStream();
    ~SoundStream();
    SoundStream& volume(float);
    SoundStream& pitch(float);
    SoundStream& position(const glm::vec4&);
    SoundStream& velocity(const glm::vec4&);
    SoundStream& direction(const glm::vec4&);
    SoundStream& cone(float inner, float outer = 360.f);
    SoundStream& attenuation(float);
    SoundStream& relativeToListener(bool r = true);
    SoundStream& loop(bool l = true);
    SoundStream& onStreamFinish(std::function<void(void)>&&);

    bool openFromFile(const std::string& filename);
    bool openFromMemory(const void* data, std::size_t sizeInBytes);
    // Time getDuration() const;
    // void setPlayingOffset(Time timeOffset);
};
}
