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

	std::shared_ptr<ceph::MoveByAction> foo;

	std::vector<std::shared_ptr<ceph::FiniteAction>> actions = {
		std::make_shared<ceph::MoveByAction>(2.0f, 100.0f, 100.0f),
		std::make_shared<ceph::MoveByAction>(1.0f, -100.0f, 100.0f),
		std::make_shared<ceph::MoveByAction>(2.0f, -100.0f, -100.0f),
		std::make_shared<ceph::MoveByAction>(1.0f, 100.0f, -100.0f),
	};

	auto seq = std::make_shared<ceph::SequenceAction>(actions);
	seq->getCompletionEvent().connect( *this,  [=](ceph::Action& action) { Test(action); } );

	ship_->applyAction(seq);

	this->addActor(ship_);
}


void Asteroids::Test(ceph::Action& action)
{
	int aaa;
	aaa = 5;
}
