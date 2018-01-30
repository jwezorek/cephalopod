#pragma once

#include "SFML/Graphics.hpp"
#include "../include/cephalopod/types.hpp"

namespace ceph {

	class Actor;

	class ActorState
	{
	private:
		sf::Transform local_to_global_transform_;
		sf::Transformable transform_;
		float alpha_;
		std::string sprite_frame_;

	public:
		ActorState(const Actor& actor);

		void translate(const ceph::Vec2D<float>& v);
		void translate(float x, float y);
		void rotate(float theta);

		void setAlpha(float alpha);
		float getAlpha() const;
		sf::Vector2f getGlobalPosition() const;
		void setGlobalPosition(const sf::Vector2f& pt);

		sf::Vector2f getPosition() const;
		float getRotation() const;
		sf::Vector2f getScale() const;

		std::string getSpriteFrame() const;
		void setSpriteFrame(const std::string& sprite_frame);
	};
}