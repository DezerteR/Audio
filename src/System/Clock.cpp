#define BOOST_CHRONO_HEADER_ONLY 1
#include <boost/chrono.hpp>
#include "Includes.hpp"
#include "Clock.hpp"

namespace Audio
{

Clock::Clock(){
	using namespace boost::chrono;
	high_resolution_clock::time_point clockNow = high_resolution_clock::now();
	auto now = time_point_cast<duration<i64, boost::ratio<1, 1000000>>>(clockNow);

	m_startTime = Time(now.time_since_epoch().count());
}

Time Clock::getElapsedTime() const {
	using namespace boost::chrono;
	high_resolution_clock::time_point clockNow = high_resolution_clock::now();
	auto now = time_point_cast<duration<i64, boost::ratio<1, 1000000>>>(clockNow);

	return Time(now.time_since_epoch().count()) - m_startTime;
}

Time Clock::restart(){
	using namespace boost::chrono;
	high_resolution_clock::time_point clockNow = high_resolution_clock::now();
	auto now = time_point_cast<duration<i64, boost::ratio<1, 1000000>>>(clockNow);

	Time elapsed = Time(now.time_since_epoch().count()) - m_startTime;
	m_startTime = Time(now.time_since_epoch().count());

	return elapsed;
}

}
