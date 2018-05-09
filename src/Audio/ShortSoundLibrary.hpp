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

public:
    ShortSoundLibrary();
    ~ShortSoundLibrary();
    void loadDirectory(const std::string&);
    SoundSource& spawn(const std::string&);
    void update(int microseconds);
    void defaultVolume(float);
};
}
