#define _USE_MATH_DEFINES
#include <cmath>
#include "actorstate.hpp"
#include "actorimpl.hpp"
#include "../include/cephalopod/actor.hpp"

ceph::ActorState::ActorState(const ceph::Actor& actor) 
{
	transform = actor.impl_->properties;
}
