#pragma once
#include "AudioPlayerUtils.hpp"

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_RADIANS
#include <vector>
#include <glm/glm.hpp>
#include "SoundSource.hpp"
#include "Listener.hpp"

typedef int32_t i32;
using std::shared_ptr;
using std::make_shared;

namespace Audio
{

class SoundBuffer;
enum SourceType
{
	Source2D, Source3D
};

class StreamedPlayer2D;
class StreamedPlayer3D;
class Sound;
class SoundStream;

enum class OnTrackFinish
{
	Repeat, Random, Next,
};
/*
class StreamedPlayer2D
{
public:
	StreamedPlayer2D();
	~StreamedPlayer2D();
	void next(u32 incr = 0);
	void play();
	void play(u32 id);
	void pause();
	void stop();
	void loop(bool l = true);

	void loadFromDirectory(const std::string&);

	StreamedPlayer2D& volume(float);
	StreamedPlayer2D& pitch(float);

	Status m_status { Stopped };
	float m_volume;
	float m_pitch;
	u32 m_current;

protected:
	void open(u32 id = 0);

	std::vector<std::string> m_playlist;
	u32 m_track;
	std::shared_ptr<SoundStream> m_music;
	std::default_random_engine RNG;
};

class StreamedPlayer3D : public StreamedPlayer2D
{
public:
	StreamedPlayer3D();
	StreamedPlayer3D& volume(float);
	StreamedPlayer3D& pitch(float);
	StreamedPlayer3D& position(const glm::vec4&);
	StreamedPlayer3D& velocity(const glm::vec4&);
	StreamedPlayer3D& attenuation(float);

	float m_attenuation;
	glm::vec4 m_position;
	glm::vec4 m_velocity;
};


class SoundPlayer2D
{
public:
	void loadFromDirectory(const std::string&);
	SoundSource& get(u32);
	SoundSource& get(const std::string&);
	void update(i32);
private:
	std::vector<SoundSource> soundSources;
	std::vector<shared_ptr<SoundBuffer>> stereoSoundBuffers;
};

class SoundPlayer3D
{
public:
	void loadFromDirectory(const std::string&);
	SoundSource& get(u32);
	SoundSource& get(const std::string&);
	void update(i32);
private:
	std::vector<SoundSource> soundSources;
	std::vector<shared_ptr<SoundBuffer>> monoSoundBuffers;
};
*/
}
