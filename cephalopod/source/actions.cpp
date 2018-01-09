#define _USE_MATH_DEFINES
#include <cmath>
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/types.hpp"
#include "actorstate.hpp"
#include "util.hpp"
#include <map>
#include <numeric>

namespace {
	
}

ceph::Action::Action(float d, ActionFunction f) : duration_(d), function_(f)
{}

float ceph::Action::getDuration() const
{
	return duration_;
}

ceph::ActionFunction ceph::Action::getFunction() const
{
	return function_;
}

void ceph::Action::operator()(ceph::ActorState & state, float t) const
{
	function_(state, t);
}


ceph::Action ceph::createMoveByAction(float duration, float x, float y)
{
	return ceph::Action(
		duration,
		[x, y](ceph::ActorState& state, float t) -> void {
			state.translate( t*x, t*y);
		}
	);
}

ceph::Action ceph::createRotateByAction(float duration, float theta)
{
	return ceph::Action(
		duration,
		[theta](ceph::ActorState& state, float t) -> void {
			state.rotate( t*theta );
		}
	);
}
