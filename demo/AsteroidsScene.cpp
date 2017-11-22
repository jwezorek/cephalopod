#include "AsteroidsScene.hpp"
#include "cephalopod/texture.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/actions.hpp"
#include "cephalopod/easingactions.hpp"

void Asteroids::initialize()
{
	sprite_sheet_ = ceph::SpriteSheet::create(
		".\\data\\zarquon.png",
		".\\data\\zarquon.json"
	);

	ceph::Game::getInstance().keyEvent.connect( *this,
		[=](bool isPressed, ceph::KeyCode key, unsigned char modifiers) {
		if (isPressed && key == ceph::KeyCode::Space)
			Test();
		}
	);

	setBackgroundColor(ceph::ColorRGB(0, 10, 20));
}


void Asteroids::Test()
{
	ship_ = std::make_shared<Ship>(sprite_sheet_);
	auto rect = ceph::Game::getInstance().getLogicalRect();
	ship_->setPosition(rect.x + rect.wd / 2.0f - 100.0f, rect.y + rect.hgt / 2.0f
	);
	ship_->applyAction(
		std::make_shared<ceph::BounceEasingAction>(
			ceph::EasingFnType::Out, 
			std::make_shared<ceph::MoveByAction>(2.0f, 0.0f, 300.0f)
		)
	);

	alien_ = std::make_shared<Alien>(sprite_sheet_);
	alien_->setPosition(rect.x + rect.wd / 2.0f + 100.0f, rect.y + rect.hgt / 2.0f);
	std::vector<std::shared_ptr<ceph::FiniteAction>> actions = {
		std::make_shared<ceph::QuadEasingAction>(ceph::EasingFnType::In, std::make_shared<ceph::MoveByAction>(1.0f, 100.0f, 100.0f)),
		std::make_shared<ceph::QuadEasingAction>(ceph::EasingFnType::Out, std::make_shared<ceph::MoveByAction>(1.0f, -100.0f, 100.0f)),
		std::make_shared<ceph::QuadEasingAction>(ceph::EasingFnType::In, std::make_shared<ceph::MoveByAction>(1.0f, -100.0f, -100.0f)),
		std::make_shared<ceph::QuadEasingAction>(ceph::EasingFnType::Out, std::make_shared<ceph::MoveByAction>(1.0f, 100.0f, -100.0f)),
	};
	auto seq = std::make_shared<ceph::SequenceAction>(actions);
	alien_->applyAction(seq);

	this->addActor(ship_);
	this->addActor(alien_);
}
