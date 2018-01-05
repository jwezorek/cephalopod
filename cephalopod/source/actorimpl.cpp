#include "actorimpl.hpp"
#include "actorstate.hpp"

ceph::ActorImpl::ActorImpl(Actor& a, const sf::Transformable & props) :
	actor(a),
	actions(a),
	alpha(1.0f),
	anchor(0,0),
	frame_sz(0,0),
	properties(props)
{
}

