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
	position(0.0f, 0.0f),
	scale(1.0f),
	rotation(0.0f),
	alpha(1.0f)
{
}

ceph::ActorState::ActorState(const Actor& actor) :
	position( actor.getPosition() ),
	scale( actor.getScale() ),
	rotation( actor.getRotation() ),
	alpha( actor.getAlpha() )
{
}

ceph::ActorState& ceph::ActorState::operator+=(ceph::Vec2D<float> position_offset)
{
	position = position + position_offset;
	return *this;
}

ceph::Action::Action(float duration) :
	state_( ceph::Action::State::Detached ),
	duration_(duration),
	is_toplevel_(false)
{
}

void ceph::Action::run(bool toplevel)
{
	if (state_ == ceph::Action::State::Detached || state_ == ceph::Action::State::Complete) {
		state_ = ceph::Action::State::Running;
		is_toplevel_ = toplevel;
		for (auto& child : children_)
			child->run(false);
	}
}

void ceph::Action::setComplete()
{
	if (isRunning()) {
		state_ = ceph::Action::State::Complete;
		for (auto& child : children_)
			child->setComplete();
	}
}

void ceph::Action::pause()
{
	state_ = ceph::Action::State::Paused;
}

void ceph::Action::unpause()
{
	if (isPaused())
		state_ = ceph::Action::State::Running;
}

float ceph::Action::getDuration() const
{
	return duration_;
}

bool ceph::Action::isPaused() const
{
	return state_ == ceph::Action::State::Paused;
}

bool ceph::Action::isRunning() const
{
	return state_ == ceph::Action::State::Running ||
		state_ == ceph::Action::State::Paused;
}

bool ceph::Action::isToplevel() const
{
	return is_toplevel_;
}

bool ceph::Action::isComplete() const
{
	return is_toplevel_;
}

ceph::Action::~Action()
{
}

/*----------------------------------------------------------------------------------------------------*/

void ceph::MoveByAction::update(ActorState& state, float t)
{
	state += (t * offset_);
}

ceph::MoveByAction::MoveByAction(float duration, float x, float y) : Action(duration), offset_(x, y)
{
}

