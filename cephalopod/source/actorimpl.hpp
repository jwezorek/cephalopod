#pragma once

#include "SFML/Graphics.hpp"
#include "../include/cephalopod/actor.hpp"

namespace ceph
{
	class ActorImpl {
	public:
		Actor* parent;
		float alpha;
		Point<float> anchor;
		Size<int> frame_sz;
		sf::Transformable properties;
		ActorImpl(const sf::Transformable& props = sf::Transformable());
	};
}