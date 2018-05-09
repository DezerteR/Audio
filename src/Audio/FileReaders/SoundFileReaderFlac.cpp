#include "SoundFileReaderFlac.hpp"
#include "InputStream.hpp"
#include <cassert>

namespace
{
	FLAC__StreamDecoderReadStatus streamRead(const FLAC__StreamDecoder*, FLAC__byte buffer[], std::size_t* bytes, void* clientData){
		Audio::SoundFileReaderFlac::ClientData* data = static_cast<Audio::SoundFileReaderFlac::ClientData*>(clientData);

		i64 count = data->stream->read(buffer, *bytes);
		if(count > 0){
			*bytes = static_cast<std::size_t>(count);
			return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
		}
		else if(count == 0){
			return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
		}
		else {
			return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
		}
	}

	FLAC__StreamDecoderSeekStatus streamSeek(const FLAC__StreamDecoder*, FLAC__uint64 absoluteByteOffset, void* clientData) {
		Audio::SoundFileReaderFlac::ClientData* data = static_cast<Audio::SoundFileReaderFlac::ClientData*>(clientData);

		i64 position = data->stream->seek(absoluteByteOffset);
		if(position >= 0)
			return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
		else
			return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
	}

	FLAC__StreamDecoderTellStatus streamTell(const FLAC__StreamDecoder*, FLAC__uint64* absoluteByteOffset, void* clientData){
		Audio::SoundFileReaderFlac::ClientData* data = static_cast<Audio::SoundFileReaderFlac::ClientData*>(clientData);

		i64 position = data->stream->tell();
		if(position >= 0){
			*absoluteByteOffset = position;
			return FLAC__STREAM_DECODER_TELL_STATUS_OK;
		}
		else {
			return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
		}
	}

	FLAC__StreamDecoderLengthStatus streamLength(const FLAC__StreamDecoder*, FLAC__uint64* streamLength, void* clientData){
		Audio::SoundFileReaderFlac::ClientData* data = static_cast<Audio::SoundFileReaderFlac::ClientData*>(clientData);

		i64 count = data->stream->getSize();
		if(count >= 0){
			*streamLength = count;
			return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
		}
		else {
			return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
		}
	}

	FLAC__bool streamEof(const FLAC__StreamDecoder*, void* clientData){
		Audio::SoundFileReaderFlac::ClientData* data = static_cast<Audio::SoundFileReaderFlac::ClientData*>(clientData);

		return data->stream->tell() == data->stream->getSize();
	}

	FLAC__StreamDecoderWriteStatus streamWrite(const FLAC__StreamDecoder*, const FLAC__Frame* frame, const FLAC__int32* const buffer[], void* clientData){
		Audio::SoundFileReaderFlac::ClientData* data = static_cast<Audio::SoundFileReaderFlac::ClientData*>(clientData);

		// If there's no output buffer, it means that we are seeking
		if(!data->buffer)
			return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;

		// Reserve memory if we're going to use the leftovers buffer
		u32 frameSamples = frame->header.blocksize * frame->header.channels;
		if(data->remaining < frameSamples)
			data->leftovers.reserve(static_cast<std::size_t>(frameSamples - data->remaining));

		// Decode the samples
		for(unsigned i = 0; i < frame->header.blocksize; ++i){
			for(u32 j = 0; j < frame->header.channels; ++j){
				// Decode the current sample
				i16 sample = 0;
				switch (frame->header.bits_per_sample){
					case 8:
						sample = buffer[j][i] << 8;
						break;
					case 16:
						sample = buffer[j][i];
						break;
					case 24:
						sample = buffer[j][i] >> 8;
						break;
					case 32:
						sample = buffer[j][i] >> 16;
						break;
					default:
						assert(false);
						break;
				}

				if(data->remaining > 0){
					// There's room in the output buffer, copy the sample there
					*data->buffer++ = sample;
					data->remaining--;
				}
				else {
					// We have decoded all the requested samples, put the sample in a temporary buffer until next call
					data->leftovers.push_back(sample);
				}
			}
		}

		return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
	}

	void streamMetadata(const FLAC__StreamDecoder*, const FLAC__StreamMetadata* meta, void* clientData){
		Audio::SoundFileReaderFlac::ClientData* data = static_cast<Audio::SoundFileReaderFlac::ClientData*>(clientData);

		if(meta->type == FLAC__METADATA_TYPE_STREAMINFO){
			data->info.sampleCount = meta->data.stream_info.total_samples * meta->data.stream_info.channels;
			data->info.sampleRate = meta->data.stream_info.sample_rate;
			data->info.channelCount = meta->data.stream_info.channels;
		}
	}

	void streamError(const FLAC__StreamDecoder*, FLAC__StreamDecoderErrorStatus, void* clientData){
		Audio::SoundFileReaderFlac::ClientData* data = static_cast<Audio::SoundFileReaderFlac::ClientData*>(clientData);
		data->error = true;
	}
}

namespace Audio
{

bool SoundFileReaderFlac::check(InputStream& stream){
	// Create a decoder
	FLAC__StreamDecoder* decoder = FLAC__stream_decoder_new();
	if(!decoder)
		return false;

	// Initialize the decoder with our callbacks
	ClientData data;
	data.stream = &stream;
	data.error = false;
	FLAC__stream_decoder_init_stream(decoder, &streamRead, &streamSeek, &streamTell, &streamLength, &streamEof, &streamWrite, nullptr, &streamError, &data);

	// Read the header
	bool valid = FLAC__stream_decoder_process_until_end_of_metadata(decoder) != 0;

	// Destroy the decoder
	FLAC__stream_decoder_finish(decoder);
	FLAC__stream_decoder_delete(decoder);

	return valid && !data.error;
}

SoundFileReaderFlac::SoundFileReaderFlac() :
m_decoder(nullptr){
}

SoundFileReaderFlac::~SoundFileReaderFlac(){
	close();
}

bool SoundFileReaderFlac::open(InputStream& stream, Info& info){
	// Create the decoder
	m_decoder = FLAC__stream_decoder_new();
	if(!m_decoder){
		cerr<< "Failed to open FLAC file (failed to allocate the decoder)" << std::endl;
		return false;
	}

	// Initialize the decoder with our callbacks
	m_clientData.stream = &stream;
	FLAC__stream_decoder_init_stream(m_decoder, &streamRead, &streamSeek, &streamTell, &streamLength, &streamEof, &streamWrite, &streamMetadata, &streamError, &m_clientData);

	// Read the header
	if(!FLAC__stream_decoder_process_until_end_of_metadata(m_decoder)){
		close();
		cerr<< "Failed to open FLAC file (failed to read metadata)" << std::endl;
		return false;
	}

	// Retrieve the sound properties
	info = m_clientData.info; // was filled in the "metadata" callback

	return true;
}

void SoundFileReaderFlac::seek(u64 sampleOffset){
	assert(m_decoder);

	// Reset the callback data (the "write" callback will be called)
	m_clientData.buffer = nullptr;
	m_clientData.remaining = 0;
	m_clientData.leftovers.clear();

	FLAC__stream_decoder_seek_absolute(m_decoder, sampleOffset);
}

u64 SoundFileReaderFlac::read(i16* samples, u64 maxCount){
	assert(m_decoder);

	// If there are leftovers from previous call, use it first
	u64 left = m_clientData.leftovers.size();
	if(left > 0){
		if(left > maxCount){
			// There are more leftovers than needed
			std::copy(m_clientData.leftovers.begin(), m_clientData.leftovers.end(), samples);
			std::vector<i16> leftovers(m_clientData.leftovers.begin() + maxCount, m_clientData.leftovers.end());
			m_clientData.leftovers.swap(leftovers);
			return maxCount;
		}
		else {
			// We can use all the leftovers and decode new frames
			std::copy(m_clientData.leftovers.begin(), m_clientData.leftovers.end(), samples);
		}
	}

	// Reset the data that will be used in the callback
	m_clientData.buffer = samples + left;
	m_clientData.remaining = maxCount - left;
	m_clientData.leftovers.clear();

	// Decode frames one by one until we reach the requested sample count, the end of file or an error
	while (m_clientData.remaining > 0){
		// Everything happens in the "write" callback
		// This will break on any fatal error (does not include EOF)
		if(!FLAC__stream_decoder_process_single(m_decoder))
			break;

		// Break on EOF
		if(FLAC__stream_decoder_get_state(m_decoder) == FLAC__STREAM_DECODER_END_OF_STREAM)
			break;
	}

	return maxCount - m_clientData.remaining;
}

void SoundFileReaderFlac::close(){
	if(m_decoder){
		FLAC__stream_decoder_finish(m_decoder);
		FLAC__stream_decoder_delete(m_decoder);
		m_decoder = nullptr;
	}
}

}
