#include "AsteroidsScene.hpp"
#include "cephalopod/texture.hpp"
#include "cephalopod/game.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

Asteroids::Asteroids() 
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
	this->addActor(ship_);
}


