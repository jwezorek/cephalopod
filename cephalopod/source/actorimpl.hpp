#pragma once

#include "SFML/Graphics.hpp"
#include "../include/cephalopod/actor.hpp"
#include "actionplayerimpl.hpp"

namespace ceph
{
	class ActorImpl {
	public:
		Actor& actor;
		Actor* parent;
		float alpha;
		Vec2D<float> anchor;
		Size<int> frame_sz;
		sf::Transformable properties;
		ActionPlayerImpl actions;

		ActorImpl(Actor& a, const sf::Transformable& props = sf::Transformable());
	};
}