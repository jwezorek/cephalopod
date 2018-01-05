#define _USE_MATH_DEFINES
#include <cmath>
#include "actorstate.hpp"
#include "../include/cephalopod/actor.hpp"

ceph::ActorState::ActorState() :
	translation_(0.0f, 0.0f),
	scale_(1.0f),
	rotation_(0.0f),
	alpha_(1.0f)
{
}

ceph::ActorState::ActorState(const Actor& actor) :
	translation_(actor.getPosition()),
	scale_(actor.getScale()),
	rotation_(actor.getRotation()),
	alpha_(actor.getAlpha())
{
}

ceph::ActorState& ceph::ActorState::applyTranslation(ceph::Vec2D<float> translation)
{
	translation_ = translation_ + translation;
	return *this;
}

ceph::ActorState& ceph::ActorState::applyRotation(float theta)
{
	rotation_ += theta;
	rotation_ = static_cast<float>(std::fmod(rotation_, 2.0*M_PI));
	return *this;
}

ceph::ActorState& ceph::ActorState::applyScale(float scale)
{
	scale_ *= scale;
	return *this;
}

ceph::ActorState& ceph::ActorState::applyAlpha(float alpha)
{
	alpha_ *= alpha_;
	return *this;
}

ceph::Vec2D<float> ceph::ActorState::getTranslation() const
{
	return translation_;
}

float ceph::ActorState::getScale() const
{
	return scale_;
}

float ceph::ActorState::getRotation() const
{
	return rotation_;
}

float  ceph::ActorState::getAlpha() const
{
	return alpha_;
}