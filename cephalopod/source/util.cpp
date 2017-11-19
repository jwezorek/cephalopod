#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>
#include "../include/cephalopod/types.hpp"
#include "util.hpp"
#include "TextureImpl.hpp"

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


