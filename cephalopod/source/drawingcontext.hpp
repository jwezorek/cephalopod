#pragma once
#include "SFML/Graphics.hpp"

namespace ceph
{
	class DrawingContext
	{
	public:
		sf::RenderTarget& target;
		sf::Transform transform;
		float alpha;

		DrawingContext(sf::RenderTarget& targ) : 
			target(targ), 
			alpha(1.0f)
		{}

		DrawingContext(sf::RenderTarget& targ, const sf::Transform& trans, float alpha = 1.0f) :
			target(targ), 
			transform(trans), 
			alpha(alpha)
		{}
	};
}