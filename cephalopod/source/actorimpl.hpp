#pragma once

#include "SFML/Graphics.hpp"
#include "../include/cephalopod/actor.hpp"
#include "actionplayer.hpp"

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
		ActionPlayer actions;

		ActorImpl(Actor& a, const sf::Transformable& props = sf::Transformable());
	};
}