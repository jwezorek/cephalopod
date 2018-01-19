#include "actorimpl.hpp"
#include "actorstate.hpp"

ceph::ActorImpl::ActorImpl(Actor& a) :
	anchor(0,0),
	frame_sz(0,0),
	alpha(1.0f)
{
}

