#pragma once

#include "SFML/Graphics.hpp"
#include "../include/cephalopod/actor.hpp"
#include "actionplayerimpl.hpp"

namespace ceph
{
	class ActorImpl {
	public:
		Vec2D<float> anchor;
		Size<int> frame_sz;
		sf::Transformable properties;
		ActionPlayerImpl actions;
		float alpha;

		ActorImpl(Actor& a);
	};
}