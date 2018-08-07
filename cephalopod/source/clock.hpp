#pragma once

#include <chrono>

namespace ceph
{
	class Clock
	{
	private:
		std::chrono::high_resolution_clock impl_;
		std::chrono::high_resolution_clock::time_point last_;

	public:
		Clock();
		float restart();
		float getElapsed() const;
	};
}