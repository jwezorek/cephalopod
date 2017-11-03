#pragma once

#include "SFML/Graphics.hpp"
#include "../include/cephalopod/actor.hpp"

namespace ceph
{
	class ActorImpl {
	public:
		Actor* parent;
		std::vector<std::shared_ptr<Actor>> children;
		float alpha;
		Point<float> anchor;
		Size<int> frame_sz;
		sf::Transformable properties;
		ActorImpl(const sf::Transformable& props = sf::Transformable());
	};
}