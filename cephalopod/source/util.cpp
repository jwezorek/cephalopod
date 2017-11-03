#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>
#include "../include/cephalopod/types.hpp"
#include "util.hpp"
#include "TextureImpl.hpp"

float ceph::RadiansToDegrees(float radians)
{
	return radians * 180.0f / static_cast<float>(M_PI);
}

float ceph::DegreesToRadians(float degrees)
{
	return degrees * static_cast<float>(M_PI) / 180.0f;
}


