#pragma once

#include <memory>
#include "cephalopod/sprite.hpp"
#include "cephalopod/scene.hpp"

class Asteroids : public ceph::Scene
{
private:
	std::shared_ptr<ceph::SpriteSheet> sprite_sheet_;
	std::shared_ptr<ceph::Sprite> ship_;

	void updateShip(float dt);

public:
	Asteroids();
};