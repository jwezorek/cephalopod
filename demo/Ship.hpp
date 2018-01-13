#pragma once

#include "cephalopod/sprite.hpp"
#include "cephalopod/spritesheet.hpp"
#include "cephalopod/events.hpp"

class Ship : public ceph::Sprite
{
public:
	Ship(const std::shared_ptr<ceph::SpriteSheet>& ss);
	void update(float dt);
	void handleKey(bool isPressed, ceph::KeyCode key, unsigned char modifiers);
};