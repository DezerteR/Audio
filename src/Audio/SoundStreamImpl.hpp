#pragma once
#include <cstdlib>
#include <atomic>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "Includes.hpp"
#include "InputSoundFile.hpp"
#include "AudioPlayerUtils.hpp"
#include "Time.hpp"

namespace Audio
{

class SoundStreamImpl
{
public:
    struct Chunk
    {
        const i16* samples;
        std::size_t  sampleCount;
    };

    SoundStreamImpl();
    ~SoundStreamImpl();
    void play();

    void pause();
    void stop();
    u32 getChannelCount() const;
    u32 getSampleRate() const;
    Status getStatus() const;
    void setPlayingOffset(Time timeOffset);
    Time getPlayingOffset() const;
    void setLoop(bool loop);
    bool getLoop() const;

    bool openFromFile(const std::string& filename);
    bool openFromMemory(const void* data, std::size_t sizeInBytes);
    Time getDuration() const;

    std::function<void(void)> m_onStreamFinish;
    u32 m_source;
    bool m_forceMono { false };

private:
    void initialize(u32 channelCount, u32 sampleRate);
    bool onGetData(Chunk& data);
    void onSeek(Time timeOffset);
    void streamData();

    bool fillAndPushBuffer(u32 bufferNum);
    bool fillQueue();
    void clearQueue();

    enum
    {
        BufferCount = 3 /// Number of audio buffers used by the streaming loop
    };

    boost::thread m_thread;
    mutable boost::mutex m_threadMutex;
    Status m_threadStartState;
    bool m_isStreaming; /// Streaming state (true = playing, false = stopped)
    u32  m_buffers[BufferCount]; /// Sound buffers used to store temporary audio data
    u32  m_channelCount;
    u32  m_sampleRate; /// Frequency (samples / second)
    u32  m_format; /// Format of the internal sound buffers
    bool m_loop; /// Loop flag (true to loop, false to play once)
    u64  m_samplesProcessed; /// Number of buffers processed since beginning of the stream
    bool m_endBuffers[BufferCount]; /// Each buffer is marked as "end buffer" or not, for proper duration calculation

    InputSoundFile m_file; /// The streamed music file
    Time m_duration; /// Music duration
    std::vector<i16> m_samples; /// Temporary buffer of samples
    mutable boost::mutex m_mutex; /// Mutex protecting the data, whyyy?
};

}
