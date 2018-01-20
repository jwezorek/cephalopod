#define _USE_MATH_DEFINES
#include <cmath>
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/types.hpp"
#include "actorstate.hpp"
#include "util.hpp"
#include <map>
#include <numeric>

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

ceph::Action ceph::createMoveByAction(float duration, const ceph::Vec2D<float>& vec)
{
	return ceph::createMoveByAction(duration, vec.x, vec.y);
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

ceph::Action ceph::createFadeByAction(float duration, float alpha)
{
	return ceph::Action(
		duration,
		[alpha](ceph::ActorState& state, float t) -> void {
			float old_alpha = state.getAlpha();
			state.setAlpha( old_alpha + t*alpha);
		}
	);
}

ceph::Action ceph::createFadeOutAction(float duration, const ceph::Actor& actor)
{
	return ceph::createFadeByAction(duration, -actor.getAlpha());
}
