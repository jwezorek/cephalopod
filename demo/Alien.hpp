#pragma once

#include "cephalopod/sprite.hpp"
#include "cephalopod/spritesheet.hpp"

class Alien : public ceph::Sprite
{
public:
	Alien(const std::shared_ptr<ceph::SpriteSheet>& ss);
};