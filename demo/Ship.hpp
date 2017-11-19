#pragma once

#include "cephalopod/sprite.hpp"
#include "cephalopod/spritesheet.hpp"

class Ship : public ceph::Sprite
{
public:
	Ship(const std::shared_ptr<ceph::SpriteSheet>& ss);
};