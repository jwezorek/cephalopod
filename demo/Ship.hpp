#pragma once

#include "cephalopod/types.hpp"
#include "cephalopod/sprite.hpp"
#include "cephalopod/events.hpp"

class Ship : public ceph::Sprite
{
	enum class RotationType {
		None = -1,
		Left = 0,
		Right = 1
	};

	bool is_thruster_on_;
	ceph::Vec2D<float> velocity_;

	RotationType getRotationState() const;
	void handleRotationKey(bool key_down, RotationType direction);
	void handleThrustKey(bool key_down);
	ceph::Vec2D<float> getDirection() const;

public:
	Ship(const std::shared_ptr<ceph::SpriteSheet>& ss);

	void update(float dt);
	void handleKey(bool isPressed, ceph::KeyCode key, unsigned char modifiers);
};