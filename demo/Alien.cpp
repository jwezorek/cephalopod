#include "Alien.hpp"
#include "cephalopod/types.hpp"

Alien::Alien(const std::shared_ptr<ceph::SpriteSheet>& ss) : ceph::Sprite(ss, "alien")
{
	ceph::Vec2D<float> pt(0.5f, 0.5f);
	setAnchorPt(pt);
}
