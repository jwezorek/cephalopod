#define _USE_MATH_DEFINES
#ifdef _WIN32
#include "Windows.h"
#endif
#include <cmath>
#include "../include/cephalopod/types.hpp"
#include "util.hpp"

float ceph::normalizeAngle(float radians)
{
	auto two_pi = 2.0f * static_cast<float>(M_PI);
	if (radians < 0.0)
		radians += two_pi;
	if (radians > two_pi)
		radians = std::fmod(radians, two_pi);
	return radians;
}

float ceph::clampValue(float value, float minimum, float maximum)
{
	if (value < minimum)
		return minimum;
	if (value > maximum)
		return maximum;
	return value;
}

float ceph::radiansToDegrees(float radians)
{
	return radians * 180.0f / static_cast<float>(M_PI);
}

float ceph::degreesToRadians(float degrees)
{
	return degrees * static_cast<float>(M_PI) / 180.0f;
}

float ceph::lerp(float start, float end, float pcnt)
{
	return (end - start) * pcnt + start;
}

ceph::Vec2<float> ceph::lerpPtInRect(const ceph::Vec2<float>& pt, const ceph::Rect<float>& r)
{
	return ceph::Vec2<float>(
		ceph::lerp(r.x, r.x2(), pt.x),
		ceph::lerp(r.y, r.y2(), pt.y)
	);
}

float ceph::distance(float x1, float y1, float x2, float y2)
{
	auto x_diff = x2 - x1;
	auto y_diff = y2 - y1;
	return std::sqrtf(x_diff * x_diff + y_diff * y_diff);
}

float ceph::magnitude(float x, float y)
{
	return distance(0, 0, x, y);
}

float ceph::magnitude(Vec2<float> vec)
{
	return magnitude(vec.x, vec.y);
}

void ceph::outputDebugString(const std::string& str)
{
	std::string line = str + std::string("\n");
#ifdef _WIN32
	OutputDebugStringA( line.c_str() );
#endif
}


