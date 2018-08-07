#include "clock.hpp"

ceph::Clock::Clock() 
{
	restart();
}

float ceph::Clock::restart()
{
	auto elapsed = getElapsed();
	last_ = impl_.now();
	return elapsed;
}

float ceph::Clock::getElapsed() const
{
	return std::chrono::duration<float>(impl_.now() - last_).count();
}