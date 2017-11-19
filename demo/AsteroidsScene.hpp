#pragma once

#include <memory>
#include "cephalopod/scene.hpp"
#include "Ship.hpp"

class Asteroids : public ceph::Scene
{

private:
	std::shared_ptr<ceph::SpriteSheet> sprite_sheet_;
	std::shared_ptr<Ship> ship_;

public:
	Asteroids();
};