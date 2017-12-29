#define _USE_MATH_DEFINES
#include <cmath>
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/types.hpp"
#include "util.hpp"
#include <map>
#include <numeric>

ceph::ActorState::ActorState() :
	translation_(0.0f, 0.0f),
	scale_(1.0f),
	rotation_(0.0f),
	alpha_(1.0f)
{
}

ceph::ActorState::ActorState(const Actor& actor) :
	translation_( actor.getPosition() ),
	scale_( actor.getScale() ),
	rotation_( actor.getRotation() ),
	alpha_( actor.getAlpha() )
{
}

ceph::ActorState& ceph::ActorState::applyTranslation(ceph::Vec2D<float> translation)
{
	translation_ = translation_ + translation;
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

/*----------------------------------------------------------------------------------------------------*/

ceph::Action::Action(float duration) : duration_(duration)
{
}

float ceph::Action::getDuration() const
{
	return duration_;
}

ceph::Action::~Action()
{
}

/*----------------------------------------------------------------------------------------------------*/

void ceph::MoveByAction::update(ActorState& state, float t) const
{
	state.applyTranslation(t * offset_);
}

ceph::MoveByAction::MoveByAction(float duration, float x, float y) : Action(duration), offset_(x, y)
{
}

