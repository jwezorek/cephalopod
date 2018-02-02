#pragma once

#include <memory>
#include "cephalopod/scene.hpp"

class Ship;
namespace ceph {
	class Group;
	class SpriteSheet;
}

class Asteroids : public ceph::Scene
{
private:
	std::shared_ptr<ceph::SpriteSheet> sprite_sheet_;
	std::shared_ptr<Ship> ship_;
	std::shared_ptr<ceph::Group> bkgd_layer_;

	std::shared_ptr<ceph::Actor> CreateStarLayer(float horz_speed, float alpha);
	void addAsteroid(const std::string& frame_prefix, int n);

public:
	void initialize() override;
	std::shared_ptr<ceph::Group> getBkgdLayer() const;
};