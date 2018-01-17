#pragma once

#include <memory>
#include "cephalopod/scene.hpp"
#include "cephalopod/actor.hpp"
#include "Ship.hpp"
#include "Alien.hpp"

class Asteroids : public ceph::Scene
{
private:
	std::shared_ptr<ceph::SpriteSheet> sprite_sheet_;
	std::shared_ptr<Ship> ship_;

	std::shared_ptr<ceph::Actor> CreateStarLayer(float horz_speed, float alpha);

public:
	void initialize() override;
};