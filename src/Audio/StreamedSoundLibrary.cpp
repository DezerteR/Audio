#include "StreamedSoundLibrary.hpp"
#include "Utils.hpp"
#include <AL/al.h>
#include <iostream>

namespace Audio
{

void StreamedSoundLibrary::next(int incr){
    m_current = (m_current+incr)%m_tracks.size();
    open(m_current);
    play();
}
void StreamedSoundLibrary::prev(int incr){
    m_current = (m_current < incr) < 0 ? m_tracks.size() + m_current-incr : m_current-incr;
    open(m_current);
    play();
}
void StreamedSoundLibrary::random(){
}
void StreamedSoundLibrary::play(){
    m_player.play();
    m_status = Playing;
}
void StreamedSoundLibrary::play(int id){
    m_current = glm::clamp(id, 0, (int)m_tracks.size());
    open(m_current);
    m_player.volume(m_volume);
    play();
}
void StreamedSoundLibrary::play(const std::string&){}
void StreamedSoundLibrary::open(int id){
    stop();
    if(!m_player.openFromFile(m_tracks[id])){
        std::cerr << "[ Error ] no file: " + m_tracks[id] << std::endl;
        stop();
        return;
    }
}
void StreamedSoundLibrary::pause(){
    m_player.pause();
    m_status = Paused;
}
void StreamedSoundLibrary::stop(){
    m_status = Stopped;
    m_player.stop();
}
void StreamedSoundLibrary::loop(bool l){}
void StreamedSoundLibrary::loadDirectory(const std::string& dir){
    m_current = 0;
    m_directory = dir;
    auto files = getFiles(m_directory, "*.wav");
    for(auto &it : files){
        m_tracks.push_back(it.localFilePath);
    }
    open(m_current);
}
void StreamedSoundLibrary::update(int microseconds){}

void StreamedSoundLibrary::volume(float v){
    m_player.volume(v);
    m_volume = v;
}
void StreamedSoundLibrary::position(const glm::vec4& p){}
void StreamedSoundLibrary::velocity(const glm::vec4& v){}
void StreamedSoundLibrary::attenuation(float a){}

int StreamedSoundLibrary::timeLeft(){
    return m_player.miliseconds();
}

}
