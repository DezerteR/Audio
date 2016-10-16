#include <boost/date_time/posix_time/posix_time.hpp>
#include "StreamedSoundSource.hpp"
#include "AudioPlayerUtils.hpp"

namespace Audio
{

void streamSoundFromThread(void(StreamedSoundSource::*function)(), StreamedSoundSource* object){
    (object->*function)();
}

StreamedSoundSource::StreamedSoundSource() :
m_thread(),
m_threadMutex(),
m_threadStartState(Stopped),
m_isStreaming(false),
m_channelCount(0),
m_sampleRate(0),
m_format(0),
m_loop(false),
m_file(),
m_duration(),
m_samplesProcessed(0){
	alGenSources(1, &m_source);
	alSourcei(m_source, AL_BUFFER, 0);
}

StreamedSoundSource::~StreamedSoundSource(){
	stop();
	{
		boost::mutex::scoped_lock lock(m_threadMutex);
		m_isStreaming = false;
	}

	m_thread.join();

	alSourcei(m_source, AL_BUFFER, 0);
	alDeleteSources(1, &m_source);
}

bool StreamedSoundSource::openFromFile(const std::string& filename){
	stop();

	// m_file.m_forceMono = m_forceMono;

	if(!m_file.openFromFile(filename)){
		cerr<<"can't open the file: "<<filename<<endl;
		return false;
	}

	m_duration = m_file.getDuration();
	m_samples.resize(m_file.getSampleRate() * m_file.getChannelCount());
	initialize(m_file.getChannelCount(), m_file.getSampleRate());

	return true;
}
bool StreamedSoundSource::openFromMemory(const void* data, std::size_t sizeInBytes){
	stop();

	m_file.m_forceMono = m_forceMono;

	if(!m_file.openFromMemory(data, sizeInBytes))
		return false;

	m_duration = m_file.getDuration();
	m_samples.resize(m_file.getSampleRate() * m_file.getChannelCount());
	initialize(m_file.getChannelCount(), m_file.getSampleRate());

	return true;
}

Status StreamedSoundSource::getStatus() const {
	ALint status;
	alGetSourcei(m_source, AL_SOURCE_STATE, &status);

	switch (status){
		case AL_INITIAL:
		case AL_STOPPED: return Stopped;
		case AL_PAUSED:  return Paused;
		case AL_PLAYING: return Playing;
	}

	return Stopped;
}

bool StreamedSoundSource::onGetData(StreamedSoundSource::Chunk& data){
	boost::mutex::scoped_lock lock(m_mutex);

	data.samples = m_samples.data();
	data.sampleCount = static_cast<std::size_t>(m_file.read(m_samples.data(), m_samples.size()));

	bool running = data.sampleCount == m_samples.size();

	if(m_forceMono){
		data.sampleCount = AudioUtils::convertToMono(m_samples.data(), data.sampleCount);
	}

	if(not running && m_onTrackEndCallback){
		m_onTrackEndCallback();
	}
	return running;
}

void StreamedSoundSource::onSeek(Time timeOffset){
	boost::mutex::scoped_lock lock(m_mutex);

	m_file.seek(timeOffset);
}

void StreamedSoundSource::initialize(u32 channelCount, u32 sampleRate){
	if(m_forceMono && channelCount == 2){
		channelCount = 1;
	}
	else m_forceMono = false;

	m_channelCount = channelCount;
	m_sampleRate   = sampleRate;

	m_format = AudioUtils::calcFormatFromChannelCount(channelCount);

	if(m_format == 0){
		m_channelCount = 0;
		m_sampleRate   = 0;
		cerr<< "Unsupported number of channels (" << m_channelCount << ")" << std::endl;
	}
}

void StreamedSoundSource::play(){
	// Check if the sound parameters have been set
	if(m_format == 0){
		cerr<< "Failed to play audio stream: sound parameters have not been initialized (call initialize() first)" << std::endl;
		return;
	}

	bool isStreaming = false;
	Status threadStartState = Stopped;

	{
		boost::mutex::scoped_lock lock(m_threadMutex);

		isStreaming = m_isStreaming;
		threadStartState = m_threadStartState;
	}

	if(isStreaming && (threadStartState == Paused)){
		// If the sound is paused, resume it
		boost::mutex::scoped_lock lock(m_threadMutex);
		m_threadStartState = Playing;
		alSourcePlay(m_source);
		return;
	}
	else if(isStreaming && (threadStartState == Playing)){
		// If the sound is playing, stop it and continue as if it was stopped
		stop();
	}

	// Move to the beginning
	onSeek(Time::Zero);

	// Start updating the stream in a separate thread to avoid blocking the application
	m_samplesProcessed = 0;
	m_isStreaming = true;
	m_threadStartState = Playing;

	m_thread = boost::thread(streamSoundFromThread, &StreamedSoundSource::streamData, this);
}
void StreamedSoundSource::pause(){
	// Handle pause() being called before the thread has started
	{
		boost::mutex::scoped_lock lock(m_threadMutex);

		if(!m_isStreaming)
			return;

		m_threadStartState = Paused;
	}

	alSourcePause(m_source);
}
void StreamedSoundSource::stop(){
	// Request the thread to terminate
	{
		boost::mutex::scoped_lock lock(m_threadMutex);
		m_isStreaming = false;
	}

	m_thread.join();
	onSeek(Time::Zero);
	m_samplesProcessed = 0;
}

u32 StreamedSoundSource::getChannelCount() const {
	return m_channelCount;
}
u32 StreamedSoundSource::getSampleRate() const {
	return m_sampleRate;
}

void StreamedSoundSource::setPlayingOffset(Time timeOffset){
	// Get old playing status
	Status oldStatus = getStatus();

	// Stop the stream
	stop();

	// Let the derived class update the current position
	onSeek(timeOffset);

	// Restart streaming
	m_samplesProcessed = static_cast<u64>(timeOffset.asSeconds() * m_sampleRate * m_channelCount);

	if(oldStatus == Stopped)
		return;

	m_isStreaming = true;
	m_threadStartState = oldStatus;

	m_thread = boost::thread(streamSoundFromThread, &StreamedSoundSource::streamData, this);
}

Time StreamedSoundSource::getPlayingOffset() const {
	if(m_sampleRate && m_channelCount){
		ALfloat secs = 0.f;
		alGetSourcef(m_source, AL_SEC_OFFSET, &secs);

		return seconds(secs + static_cast<float>(m_samplesProcessed) / m_sampleRate / m_channelCount);
	}
	else
	{
		return Time::Zero;
	}
}

void StreamedSoundSource::setLoop(bool loop){
	m_loop = loop;
}

bool StreamedSoundSource::getLoop() const {
	return m_loop;
}

void StreamedSoundSource::streamData(){
	bool requestStop = false;

	{
		boost::mutex::scoped_lock lock(m_threadMutex);

		// Check if the thread was launched Stopped
		if(m_threadStartState == Stopped)
		{
			m_isStreaming = false;
			return;
		}
	}

	// Create the buffers
	alGenBuffers(BufferCount, m_buffers);
	for(int i = 0; i < BufferCount; ++i)
		m_endBuffers[i] = false;

	// Fill the queue
	requestStop = fillQueue();

	// Play the sound
	alSourcePlay(m_source);

	{
		boost::mutex::scoped_lock lock(m_threadMutex);

		// Check if the thread was launched Paused
		if(m_threadStartState == Paused)
			alSourcePause(m_source);
	}

	for(;;){
		{
			boost::mutex::scoped_lock lock(m_threadMutex);
			if(!m_isStreaming)
				break;
		}

		// The stream has been interrupted!
		if(getStatus() == Stopped)
		{
			if(!requestStop)
			{
				// Just continue
				alSourcePlay(m_source);
			}
			else
			{
				// End streaming
				boost::mutex::scoped_lock lock(m_threadMutex);
				m_isStreaming = false;
			}
		}

		// Get the number of buffers that have been processed (i.e. ready for reuse)
		ALint nbProcessed = 0;
		alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &nbProcessed);

		while (nbProcessed--)
		{
			// Pop the first unused buffer from the queue
			ALuint buffer;
			alSourceUnqueueBuffers(m_source, 1, &buffer);

			// Find its number
			u32 bufferNum = 0;
			for(int i = 0; i < BufferCount; ++i)
				if(m_buffers[i] == buffer)
				{
					bufferNum = i;
					break;
				}

			// Retrieve its size and add it to the samples count
			if(m_endBuffers[bufferNum])
			{
				// This was the last buffer: reset the sample count
				m_samplesProcessed = 0;
				m_endBuffers[bufferNum] = false;
			}
			else
			{
				ALint size, bits;
				alGetBufferi(buffer, AL_SIZE, &size);
				alGetBufferi(buffer, AL_BITS, &bits);

				// Bits can be 0 if the format or parameters are corrupt, avoid division by zero
				if(bits == 0)
				{
					cerr<< "Bits in sound stream are 0: make sure that the audio format is not corrupt "
						  << "and initialize() has been called correctly" << std::endl;

					// Abort streaming (exit main loop)
					boost::mutex::scoped_lock lock(m_threadMutex);
					m_isStreaming = false;
					requestStop = true;
					break;
				}
				else
				{
					m_samplesProcessed += size / (bits / 8);
				}
			}

			// Fill it and push it back into the playing queue
			if(!requestStop)
			{
				if(fillAndPushBuffer(bufferNum))
					requestStop = true;
			}
		}

		// Leave some time for the other threads if the stream is still playing
		if(getStatus() != Stopped)
			boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
			// sleep(milliseconds(10));
	}

	// Stop the playback
	alSourceStop(m_source);

	// Dequeue any buffer left in the queue
	clearQueue();

	// Delete the buffers
	alSourcei(m_source, AL_BUFFER, 0);
	alDeleteBuffers(BufferCount, m_buffers);
}

bool StreamedSoundSource::fillAndPushBuffer(u32 bufferNum){
	bool requestStop = false;

	// Acquire audio data
	Chunk data = {nullptr, 0};
	if(!onGetData(data)){
		// Mark the buffer as the last one (so that we know when to reset the playing position)
		m_endBuffers[bufferNum] = true;

		// Check if the stream must loop or stop
		if(m_loop){
			// Return to the beginning of the stream source
			onSeek(Time::Zero);

			// If we previously had no data, try to fill the buffer once again
			if(!data.samples || (data.sampleCount == 0)){
				return fillAndPushBuffer(bufferNum);
			}
		}
		else {
			// Not looping: request stop
			requestStop = true;
		}
	}

	// Fill the buffer if some data was returned
	if(data.samples && data.sampleCount){
		u32 buffer = m_buffers[bufferNum];

		// Fill the buffer
		ALsizei size = static_cast<ALsizei>(data.sampleCount) * sizeof(i16);
		alBufferData(buffer, m_format, data.samples, size, m_sampleRate);

		// Push it into the sound queue
		alSourceQueueBuffers(m_source, 1, &buffer);
	}

	return requestStop;
}

bool StreamedSoundSource::fillQueue(){
	// Fill and enqueue all the available buffers
	bool requestStop = false;
	for(int i = 0; (i < BufferCount) && !requestStop; ++i){
		if(fillAndPushBuffer(i))
			requestStop = true;
	}

	return requestStop;
}

void StreamedSoundSource::clearQueue(){
	// Get the number of buffers still in the queue
	ALint nbQueued;
	alGetSourcei(m_source, AL_BUFFERS_QUEUED, &nbQueued);

	// Dequeue them all
	ALuint buffer;
	for(ALint i = 0; i < nbQueued; ++i)
		alSourceUnqueueBuffers(m_source, 1, &buffer);
}

}
