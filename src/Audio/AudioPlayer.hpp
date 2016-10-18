#pragma once
#include "AudioPlayerUtils.hpp"

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_RADIANS
#include <vector>
#include <glm/glm.hpp>

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
class StreamedSoundSource;

enum class OnTrackFinish
{
	Repeat, Random, Next,
};

/**
 *  Designed to play background music
 */
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
	std::shared_ptr<StreamedSoundSource> m_music;
	std::default_random_engine RNG;
};

/**
 *  To play music in 3D, dialogs etc.
 *
 */
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

class SoundSource
{
public:
	SoundSource() : m_source(0), m_lifetime(0){}
	SoundSource(u32 id) : m_source(id){}
	void play();

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
	SoundSource& lifetime(i32);
	/// offset to sound start time, in miliseconsds[ms]
	SoundSource& offset(i32);
	Status status() const;

	/// getters
	glm::vec4 getPosition() const;
	glm::vec4 getVelocity() const;
	glm::vec4 getDirection() const;

	float getInnerCone() const;
	float getOuterCone() const;
	float getPitch() const;
	float getVolume() const;
	float getAttenuation() const;
	float getMinDistance() const;
	bool getLoop() const;
	bool isRelativeToListener() const;

	i32 m_lifetime;
	u32 m_source;
};

/**
 * Fire and forget player, to short sounds,
 * after that we are interested only in delete source
 * *PL* mo�naby zrobi� te metody statyczne, nie ma sensu trzyma� obiektu tej klasy,
 * skoro zestaw danych b�dzie jeden, nie?
 *
 * mo�na wi�c tablic� z soungSourcami zrobi� globaln� w .cpp, i z tamt�d wszystko wyci�ga�
 * czyli jeden globalny bufor z albuforami dostepnymi przez kilka funkcji/metody
 * i globalny bufor aktywnych �r�de� d�wi�ku
 * @usage
 *
 *  SoundPlayer3D player(Audio::GUNSHOT);
 *  player.position()
 *        .velocity();
 *        .direction()
 *        .cone()
 *        .play();
 */
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

/**
 *  Default attenuation model is: Inverse distance clamped
 *  i think there is no need to change that.
 *
 */
class Listener
{
public:
	void position(const glm::vec4&);
	void velocity(const glm::vec4&);
	void orientation(const glm::mat4&);
	void orientation(const glm::vec4&, const glm::vec4&);
	void up(const glm::vec4&);
	void at(const glm::vec4&);
	void volume(float);

	float m_volume;
	glm::vec4 m_at;
	glm::vec4 m_up;
};

class ContextHandler
{
public:
	ContextHandler();
	~ContextHandler();
};

}
