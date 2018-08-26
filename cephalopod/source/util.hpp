#pragma once
#include <memory>
#include <string>
#include <array>
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/types.hpp"

namespace ceph {
	void outputDebugString(const std::string& str);
	float clampValue(float value, float minium, float maximum);
	float normalizeAngle(float radians);
	float radiansToDegrees(float radians);
	float degreesToRadians(float degrees);
	float lerp(float start, float end, float pcnt);
	float distance(float x1, float y1, float x2, float y2);
	float magnitude(float x, float y);
	float magnitude(Vec2<float> vec);
	Vec2<float> lerpPtInRect(const Vec2<float>& pt, const Rect<float>& r);
};