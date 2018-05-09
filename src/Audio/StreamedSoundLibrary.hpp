#pragma once
#include "SoundStream.hpp"
#include <vector>

namespace Audio
{

class StreamedSoundLibrary
{
private:
    enum Status {Stopped, Playing, Paused};
    Status m_status {Stopped};
    SoundStream m_player; //delay creation?
    std::string m_directory;
    std::vector<std::string> m_tracks;
    bool m_forceMono;
    int m_current {0};
    float m_attenuation;
    float m_volume;
    glm::vec4 m_position;
    glm::vec4 m_velocity;

    void open(int id);
public:
    void mono(bool m=true){
        m_forceMono = true;
    }
    void next(int incr = 1);
    void prev(int incr = 1);
    void random();
    void play();
    void play(int id);
    void play(const std::string&);
    void pause();
    void stop();
    void loop(bool l = true);
    void loadDirectory(const std::string&);
    void update(int microseconds);
    void volume(float);
    void position(const glm::vec4&);
    void velocity(const glm::vec4&);
    void attenuation(float);
    int timeLeft();
};

};
