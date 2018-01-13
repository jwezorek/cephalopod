#include "Ship.hpp"
#include "cephalopod/types.hpp"
#include "cephalopod/game.hpp"

Ship::Ship(const std::shared_ptr<ceph::SpriteSheet>& ss) : ceph::Sprite(ss, "ship" )
{
	ceph::Point<float> pt(0.5f, 0.5f);
	setAnchorPt(pt);

	auto& key_evt = ceph::Game::getInstance().keyEvent;
	connect( key_evt, &Ship::handleKey);
}

void Ship::update(float dt)
{
	auto position = getPosition();
	setPosition(
		position.x,
		position.y + dt * 100
	);
}


void Ship::handleKey(bool isPressed, ceph::KeyCode key, unsigned char modifiers)
{
	if (key == ceph::KeyCode::Left) {
		if (isPressed) {
			int aaa;
			aaa = 5;
		} else {

		}
	} else if (key == ceph::KeyCode::Right) {

	}
}
