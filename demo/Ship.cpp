#include "Ship.hpp"
#include "cephalopod/types.hpp"

Ship::Ship(const std::shared_ptr<ceph::SpriteSheet>& ss) : ceph::Sprite(ss, "ship" )
{
	ceph::Point<float> pt(0.5f, 0.5f);
	setAnchorPt(pt);
}
