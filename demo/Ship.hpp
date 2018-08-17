#pragma once

#include "cephalopod/types.hpp"
#include "cephalopod/sprite.hpp"
#include "cephalopod/events.hpp"
#include "cephalopod/group.hpp"

class Ship : public ceph::Group
{
	enum class RotationType {
		None = -1,
		Left = 0,
		Right = 1
	};
	static constexpr int FLYING_BULLET_ACTION = 2;

	std::shared_ptr<ceph::SpriteSheet> sprites_;
	bool is_thruster_on_;
	ceph::Vec2<float> velocity_;

	RotationType getRotationState() const;
	void handleRotationKey(bool key_down, RotationType direction);
	void handleThrustKey(bool key_down);
	ceph::Vec2<float> getDirection() const;
	void shoot();

public:

	Ship(const std::shared_ptr<ceph::SpriteSheet>& ss);
	void initialize();
	void update(float dt);
	void handleKey(bool isPressed, ceph::KeyCode key, unsigned char modifiers);
};