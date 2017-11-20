#include "AsteroidsScene.hpp"
#include "cephalopod/texture.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/actions.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

void Asteroids::initialize()
{
	sprite_sheet_ = ceph::SpriteSheet::create(
		".\\data\\zarquon.png",
		".\\data\\zarquon.json"
	);

	ship_ = std::make_shared<Ship>(sprite_sheet_);

	auto rect = ceph::Game::getInstance().getLogicalRect();

	ship_->setPosition(
		rect.x + rect.wd / 2.0f,
		rect.y + rect.hgt / 2.0f
	);

	ship_->applyAction( std::make_shared<ceph::MoveByAction>(4.0f, ceph::Size<float>(100.0f, 100.0f)) );
	ship_->applyAction(std::make_shared<ceph::SetTransparencyToAction>(4.0f, 0));

	this->addActor(ship_);
}

