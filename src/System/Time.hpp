#pragma once
#include "Includes.hpp"
namespace Audio
{

class Time
{
public:

	Time();
	Time(i64 microseconds);

	float asSeconds() const;
	i32 asMilliseconds() const;
	i64 asMicroseconds() const;
	static const Time Zero; /// Predefined "zero" time value

private:

	friend Time seconds(float);
	friend Time milliseconds(i32);
	friend Time microseconds(i64);

private:
	i64 m_microseconds;
};

Time seconds(float amount);
Time milliseconds(i32 amount);
Time microseconds(i64 amount);
bool operator ==(Time left, Time right);
bool operator !=(Time left, Time right);
bool operator <(Time left, Time right);
bool operator >(Time left, Time right);
bool operator <=(Time left, Time right);
bool operator >=(Time left, Time right);
Time operator -(Time right);
Time operator +(Time left, Time right);
Time& operator +=(Time& left, Time right);
Time operator -(Time left, Time right);
Time& operator -=(Time& left, Time right);
Time operator *(Time left, float right);
Time operator *(Time left, i64 right);
Time operator *(float left, Time right);
Time operator *(i64 left, Time right);
Time& operator *=(Time& left, float right);
Time& operator *=(Time& left, i64 right);
Time operator /(Time left, float right);
Time operator /(Time left, i64 right);
Time& operator /=(Time& left, float right);
Time& operator /=(Time& left, i64 right);
float operator /(Time left, Time right);
Time operator %(Time left, Time right);
Time& operator %=(Time& left, Time right);

}
