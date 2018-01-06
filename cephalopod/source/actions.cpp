#define _USE_MATH_DEFINES
#include <cmath>
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/types.hpp"
#include "actorstate.hpp"
#include "util.hpp"
#include <map>
#include <numeric>

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
	auto amount = t * offset_;
	state.translate(amount.x, amount.y);
}

ceph::MoveByAction::MoveByAction(float duration, float x, float y) : Action(duration), offset_(x, y)
{
}

ceph::RotateByAction::RotateByAction(float duration, float theta) : Action(duration), theta_(theta)
{
}

void ceph::RotateByAction::update(ActorState & state, float t) const
{
	state.rotate( t * theta_ );
}

