#pragma once
#include "SoundStream.hpp"
#include <vector>

namespace Audio
{

class StreamedSoundLibrary
{
private:
    SoundStream m_player; //delay creation?
    std::string m_directory;
    std::vector<std::string> m_tracks;
public:
    void next(u32 incr = 0);
    void play();
    void play(u32 id);
    void play(const std::string&);
    void pause();
    void stop();
    void loop(bool l = true);
    void loadDirectory(const std::string&);
    void update(int microseconds);
};

};
