#pragma once
#include "Includes.hpp"

namespace Audio
{

enum Status
{
	Stopped,
	Paused,
	Playing
};

namespace AudioUtils
{

u32 calcFormatFromChannelCount(u32);
void convertToMono(std::vector<i16> &samples);
u64 convertToMono(i16 *samples, u64 count);

}
}
