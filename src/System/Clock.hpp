#pragma once
#include "System/Time.hpp"

namespace Audio
{

class Clock
{
public:
	Clock();
	Time getElapsedTime() const;
	Time restart();

private:
	Time m_startTime;
};

}

