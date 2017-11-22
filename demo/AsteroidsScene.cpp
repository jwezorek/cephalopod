#include "AsteroidsScene.hpp"
#include "cephalopod/texture.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/actions.hpp"
#include "cephalopod/easingactions.hpp"
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
	auto drop = std::make_shared<ceph::MoveByAction>(2, 0, 300.0);
	auto ease = std::make_shared<ceph::BounceEasingAction>(ceph::EasingFnType::Out, drop);
	ship_->applyAction(ease);

	
	alien_ = std::make_shared<Alien>(sprite_sheet_); 
	alien_->setPosition(
		rect.x + rect.wd / 2.0f + 50.9f,
		rect.y + rect.hgt / 2.0f
	);
	std::vector<std::shared_ptr<ceph::FiniteAction>> actions = {
		std::make_shared<ceph::BackEasingAction>(ceph::EasingFnType::In, std::make_shared<ceph::MoveByAction>(1.0f, 100.0f, 100.0f)),
		std::make_shared<ceph::BackEasingAction>(ceph::EasingFnType::In, std::make_shared<ceph::MoveByAction>(1.0f, -100.0f, 100.0f)),
		std::make_shared<ceph::CircEasingAction>(ceph::EasingFnType::In, std::make_shared<ceph::MoveByAction>(2.0f, -100.0f, -100.0f)),
		std::make_shared<ceph::CircEasingAction>(ceph::EasingFnType::Out, std::make_shared<ceph::MoveByAction>(2.0f, 100.0f, -100.0f)),
	};
	auto seq = std::make_shared<ceph::SequenceAction>(actions);
	seq->getCompletionEvent().connect( *this,  [=](ceph::Action& action) { Test(action); } );
	alien_->applyAction(seq);

	this->addActor( ship_ );
	this->addActor( alien_ );
}


void Asteroids::Test(ceph::Action& action)
{
	int aaa;
	aaa = 5;
}
