#pragma once

#include "AudioPlayerUtils.hpp"
#include "AudioPlayer.hpp"

class IAudio : private boost::noncopyable
{
public:
	Audio::StreamedPlayer2D music;
	Audio::StreamedPlayer3D ambient;
	Audio::SoundPlayer2D gui;
	Audio::SoundPlayer3D effects;
	Audio::SoundPlayer3D vehicle;
	Audio::Listener listener;

	void init();
private:
	Audio::ContextHandler audioContext;
};

extern unique_ptr<IAudio> audio;
