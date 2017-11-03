#include "actorimpl.hpp"

ceph::ActorImpl::ActorImpl(const sf::Transformable & props) :
	alpha(1.0f),
	anchor(0,0),
	frame_sz(0,0),
	properties(props)
{
}

