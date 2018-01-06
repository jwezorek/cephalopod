#pragma once

#include "SFML/Graphics.hpp"

namespace ceph {

	class Actor;

	class ActorState
	{
	public:
		sf::Transformable parent_transform;
		sf::Transformable transform;
		ActorState(const Actor& actor);
	};
}