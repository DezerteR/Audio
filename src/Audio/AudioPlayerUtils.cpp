#include "AudioPlayerUtils.hpp"
#include <AL/alc.h>

namespace Audio
{
namespace AudioUtils
{

u32 calcFormatFromChannelCount(u32 channelCount){
	u32 format = 0;
	switch (channelCount){
		case 1:  format = AL_FORMAT_MONO16; break;
		case 2:  format = AL_FORMAT_STEREO16; break;
		case 4:  format = alGetEnumValue("AL_FORMAT_QUAD16");  break;
		case 6:  format = alGetEnumValue("AL_FORMAT_51CHN16"); break;
		case 7:  format = alGetEnumValue("AL_FORMAT_61CHN16"); break;
		case 8:  format = alGetEnumValue("AL_FORMAT_71CHN16"); break;
		default: format = 0; break;
	}

	return format;
}

void convertToMono(std::vector<i16> &samples){
	u32 monoCount = samples.size()/2;
	std::vector<i16> monoChannel(monoCount);
	for(u32 i = 0; i < monoCount; i++){
		monoChannel[i] = (samples[i*2] + samples[i*2+1])/2;
	}
	samples.swap(monoChannel);
}

u64 convertToMono(i16 *samples, u64 count){
	// cout<<count<<endl;
	u64 monoCount = count/2;
	std::vector<i16> monoChannel(monoCount);
	for(u32 i = 0; i < monoCount; i++){
		samples[i] = (samples[i*2] + samples[i*2+1])/2;
	}
	return monoCount;
}


}
}
