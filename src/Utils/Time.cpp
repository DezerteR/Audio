#include "Time.hpp"

namespace Audio
{
const Time Time::Zero;

Time::Time() :
m_microseconds(0){
}

float Time::asSeconds() const {
	return m_microseconds / 1000000.f;
}

i32 Time::asMilliseconds() const {
	return static_cast<i32>(m_microseconds / 1000);
}

i64 Time::asMicroseconds() const {
	return m_microseconds;
}

Time::Time(i64 microseconds) :
m_microseconds(microseconds){
}

Time seconds(float amount){
	return Time(static_cast<i64>(amount * 1000000));
}

Time milliseconds(i32 amount){
	return Time(static_cast<i64>(amount) * 1000);
}

Time microseconds(i64 amount){
	return Time(amount);
}

bool operator ==(Time left, Time right){
	return left.asMicroseconds() == right.asMicroseconds();
}

bool operator !=(Time left, Time right){
	return left.asMicroseconds() != right.asMicroseconds();
}

bool operator <(Time left, Time right){
	return left.asMicroseconds() < right.asMicroseconds();
}

bool operator >(Time left, Time right){
	return left.asMicroseconds() > right.asMicroseconds();
}

bool operator <=(Time left, Time right){
	return left.asMicroseconds() <= right.asMicroseconds();
}

bool operator >=(Time left, Time right){
	return left.asMicroseconds() >= right.asMicroseconds();
}

Time operator -(Time right){
	return microseconds(-right.asMicroseconds());
}

Time operator +(Time left, Time right){
	return microseconds(left.asMicroseconds() + right.asMicroseconds());
}

Time& operator +=(Time& left, Time right){
	return left = left + right;
}

Time operator -(Time left, Time right){
	return microseconds(left.asMicroseconds() - right.asMicroseconds());
}

Time& operator -=(Time& left, Time right){
	return left = left - right;
}

Time operator *(Time left, float right){
	return seconds(left.asSeconds() * right);
}

Time operator *(Time left, i64 right){
	return microseconds(left.asMicroseconds() * right);
}

Time operator *(float left, Time right){
	return right * left;
}

Time operator *(i64 left, Time right){
	return right * left;
}

Time& operator *=(Time& left, float right){
	return left = left * right;
}

Time& operator *=(Time& left, i64 right){
	return left = left * right;
}

Time operator /(Time left, float right){
	return seconds(left.asSeconds() / right);
}

Time operator /(Time left, i64 right){
	return microseconds(left.asMicroseconds() / right);
}

Time& operator /=(Time& left, float right){
	return left = left / right;
}

Time& operator /=(Time& left, i64 right){
	return left = left / right;
}

float operator /(Time left, Time right){
	return left.asSeconds() / right.asSeconds();
}

Time operator %(Time left, Time right){
	return microseconds(left.asMicroseconds() % right.asMicroseconds());
}

Time& operator %=(Time& left, Time right){
	return left = left % right;
}

}
