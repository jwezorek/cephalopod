#define _USE_MATH_DEFINES
#include <cmath>
#include <Windows.h>
#include "easingfunctions.hpp"

const float PI = static_cast<float>(M_PI);

float ceph::ease::Back::in(float t)
{
	float s = 1.70158f;
	return t*t*((s + 1)*t - s);
}

float ceph::ease::Back::out(float t)
{
	float s = 1.70158f;
	t -= 1;
	return t*t*((s + 1)*t + s) + 1;
}

float ceph::ease::Back::inOut(float t)
{
	float s = 1.70158f;
	t *= 2.0f;
	if (t < 1)
		return 1.0f / 2.0f * (t*t*(((s *= (1.525f)) + 1.0f)*t - s));
	t -= 2;
	return 1.0f / 2.0f * (t*t*(((s *= (1.525f)) + 1.0f)*t + s) + 2.0f);
}

float ceph::ease::Bounce::in(float t)
{
	return 1.0f - out(1.0f - t);
}

float ceph::ease::Bounce::out(float t)
{
	if (t  < (1.0f / 2.75f)) {
		return 7.5625f*t*t;
	}
	else if (t < (2 / 2.75f)) {
		t -= (1.5f / 2.75f);
		return 7.5625f*t*t + .75f;
	}
	else if (t < (2.5 / 2.75)) {
		t -= (2.25f / 2.75f);
		return 7.5625f*t*t + .9375f;
	}
	else {
		t -= (2.625f / 2.75f);
		return 7.5625f*t*t + .984375f;
	}
}

float ceph::ease::Bounce::inOut(float t)
{
	if (t < 0.5f)
		return in(t * 2.0f) * 0.5f;
	else
		return 0.5f * out(2.0f * t - 1.0f) + 0.5f;
}

float ceph::ease::Circ::in(float t)
{
	return -1.0f * (std::sqrtf(1 - t*t) - 1);
}

float ceph::ease::Circ::out(float t)
{
	t -= 1.0f;
	return std::sqrtf(1.0f - t*t);
}

float ceph::ease::Circ::inOut(float t)
{
	t *= 2.0f;
	if (t < 1) {
		return -0.5f * (std::sqrtf(1 - t*t) - 1);
	}
	else {
		t -= 2.0f;
		return 0.5f * (std::sqrtf(1 - t*t) + 1);
	}
}

float ceph::ease::Cubic::in(float t)
{
	return t*t*t;
}

float ceph::ease::Cubic::out(float t)
{
	t -= 1.0f;
	return t*t*t + 1.0f;
}

float ceph::ease::Cubic::inOut(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t*t*t;
	}
	else {
		t -= 2.0f;
		return 0.5f * (t*t*t + 2.0f);
	}
}

float ceph::ease::Elastic::in(float t)
{
	if (t == 0)
		return 0;
	if (t == 1)
		return 1;
	float p = 0.3f;
	float s = p / 4;
	t -= 1;
	return -(std::pow(2.0f, 10.0f * t) * std::sin((t - s)*(2 * PI) / p));
}

float ceph::ease::Elastic::inOut(float t)
{
	if (t == 0)
		return 0;

	if ((t *= 2.0f) == 2.0f)
		return 1;

	float p = 0.3f * 1.5f;
	float s = p / 4;
	float t_minus_1 = t - 1.0f;

	if (t < 1)
		return -0.5f*(std::powf(2.0f, 10.0f * t_minus_1) * std::sinf((t_minus_1 - s)*(2.0f * PI) / p));
	else
		return std::powf(2.0f, -10.0f * t_minus_1) * std::sinf((t_minus_1 - s)*(2.0f * PI) / p)*0.5f + 1.0f;
}

float ceph::ease::Elastic::out(float t) {
	if (t == 0)
		return 0.0f;
	if (t == 1)
		return 1.0f;
	float p = 0.3f;
	float s = p / 4.0f;
	return (std::powf(2.0f, -10.0f * t) * std::sinf((t - s)*(2.0f * PI) / p) + 1.0f);
}

float ceph::ease::Expo::in(float t)
{
	return (t == 0.0f) ? 0.0f : std::powf(2.0f, 10.0f * (t - 1.0f));
}

float ceph::ease::Expo::out(float t)
{
	return (t == 1.0f) ? 1.0f : -std::powf(2.0f, -10.0f * t) + 1.0f;
}

float ceph::ease::Expo::inOut(float t)
{
	if (t == 0.0f)
		return 0.0f;
	if (t == 1.0f)
		return 1.0f;
	t *= 2.0f;
	float e = 10 * (t - 1);
	if (t < 1.0f)
		return 0.5f * std::powf(2.0f, e);
	else
		return 0.5f * (-std::powf(2.0f, -e) + 2);
}

float ceph::ease::Quad::in(float t)
{
	return t*t;
}

float ceph::ease::Quad::out(float t)
{
	return 2 * t - t*t;
}

float ceph::ease::Quad::inOut(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f*(t*t);
	}
	else {
		t -= 1.0f;
		return t + 0.5f - 0.5f*t*t;
	}
}

float ceph::ease::Quart::in(float t)
{
	return t*t*t*t;
}

float ceph::ease::Quart::out(float t)
{
	t -= 1.0f;
	return 1.0f - t*t*t*t;
}

float ceph::ease::Quart::inOut(float t)
{
	t *= 2.0f;
	if (t < 1.0f) {
		return 0.5f * t*t*t*t;
	}
	else {
		t -= 2.0f;
		return 1.0f - 0.5f * t*t*t*t;
	}
}

float ceph::ease::Quint::in(float t)
{
	return t*t*t*t*t;
}

float ceph::ease::Quint::out(float t)
{
	t -= 1.0f;
	return t*t*t*t*t + 1;
}

float ceph::ease::Quint::inOut(float t)
{
	t *= 2.0f;
	if (t  < 1.0f) {
		return 0.5f * t*t*t*t*t;
	}
	else {
		t -= 2.0f;
		return 0.5f * t*t*t*t*t + 1.0f;
	}
}

float ceph::ease::Sine::in(float t)
{
	return -1.0f * std::cosf(t * (PI / 2.0f)) + 1.0f;
}

float ceph::ease::Sine::out(float t)
{
	return std::sinf(t * (PI / 2.0f));
}

float ceph::ease::Sine::inOut(float t)
{
	return -0.5f * (std::cosf(t*PI) - 1);
}