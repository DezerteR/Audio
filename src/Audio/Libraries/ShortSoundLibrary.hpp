#pragma once
#include <string>
#include <vector>
#include <map>
#include "SoundSource.hpp"

namespace Audio
{
class SoundBuffer;

class ShortSoundLibrary
{
private:
    std::vector<SoundSource> m_activeSurces;
    std::map<std::string, std::shared_ptr<SoundBuffer>> m_loadedClips;
    std::vector<std::shared_ptr<SoundBuffer>> m_loadedClipsLinear;
    bool m_forceMono {false};
    float m_volume {1.f};
public:
    ShortSoundLibrary();
    ~ShortSoundLibrary();
    void loadDirectory(const std::string&);
    SoundSource& spawn(const std::string&);
    void update(int microseconds);
    void defaultVolume(float);
    void mono(bool m = true);
};
}
