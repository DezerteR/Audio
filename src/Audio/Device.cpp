#include "Device.hpp"
#include <AL/alc.h>
#include <iostream>

namespace Audio
{

ALCdevice*  audioDevice  = nullptr;
ALCcontext* audioContext = nullptr;

bool Device::init(){
    audioDevice = alcOpenDevice(nullptr);

    if(!audioDevice){
        std::cerr << "Failed to open the audio device" << std::endl;
        return false;
    }

    audioContext = alcCreateContext(audioDevice, nullptr);

    if(!audioContext){
        std::cerr << "Failed to create the audio context" << std::endl;
        return false;
    }
    alcMakeContextCurrent(audioContext);
    std::cout << "audio device created\n";
    return true;
}

Device::~Device(){
    alcMakeContextCurrent(nullptr);
    if(audioContext)
        alcDestroyContext(audioContext);

    if(audioDevice)
        alcCloseDevice(audioDevice);
}

};
