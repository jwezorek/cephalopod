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
		//,true
	);

	ceph::Game::getInstance().keyEvent.connect( *this,
		[=](bool isPressed, ceph::KeyCode key, unsigned char modifiers) {
		if (isPressed && key == ceph::KeyCode::Space)
			Test();
		}
	);

	setBackgroundColor(ceph::ColorRGB(0, 20, 40));
}

auto CreateTestSprite(const std::shared_ptr<ceph::SpriteSheet>& sprite_sheet, std::shared_ptr<ceph::Sprite>* ptr1, std::shared_ptr<ceph::Sprite>* ptr2)
{
	auto parent = std::make_shared<ceph::Sprite>(sprite_sheet, "test1");
	*ptr1 = std::make_shared<ceph::Sprite>(sprite_sheet, "test3");
	*ptr2 = std::make_shared<ceph::Sprite>(sprite_sheet, "test3");

	(*ptr1)->setScale(0.5f);
	(*ptr1)->setPosition(0.0f, 0.0f);

	(*ptr2)->setScale(0.5f);
	(*ptr2)->setPosition(20.0f, 20.0f);

	parent->addChild(*ptr1);
	parent->addChild(*ptr2);

	return parent;
}

auto CreateTestSprite2(const std::shared_ptr<ceph::SpriteSheet>& sprite_sheet, 
	std::shared_ptr<ceph::Sprite>* ptr1, std::shared_ptr<ceph::Sprite>* ptr2, std::shared_ptr<ceph::Sprite>* ptr3, std::shared_ptr<ceph::Sprite>* ptr4)
{
	auto parent = std::make_shared<ceph::Sprite>(sprite_sheet, "test1");
	auto child1 = CreateTestSprite(sprite_sheet, ptr1, ptr2);
	auto child2 = CreateTestSprite(sprite_sheet, ptr3, ptr4);

	child1->setScale(0.5f);
	child1->setPosition(0.0f, 0.0f);

	child2->setScale(0.5f);
	child2->setPosition(20.0f, 20.0f);

	parent->addChild(child1);
	parent->addChild(child2);

	return parent;
}


void Asteroids::Test()
{
	auto& game = ceph::Game::getInstance();
	std::shared_ptr<ceph::Sprite> ptr1; 
	std::shared_ptr<ceph::Sprite> ptr2;
	std::shared_ptr<ceph::Sprite> ptr3;
	std::shared_ptr<ceph::Sprite> ptr4;
	auto test = CreateTestSprite2(sprite_sheet_, &ptr1, &ptr2, &ptr3, &ptr4);

	test->setPosition(-250, 210);

	auto bnds = test->getGlobalBounds();
	auto scr_bnds = game.convertToScreenCoords(bnds);

	auto r = test->getLocalBounds();

	test->setPosition(100, 100);
	test->setRotationDegrees(10.0f);

	auto global1 = ptr1->getGlobalBounds();
	auto global2 = ptr2->getGlobalBounds();
	auto global3 = ptr3->getGlobalBounds();
	auto global4 = ptr4->getGlobalBounds();

	ceph::Game::getInstance().addDebugRect( game.convertToScreenCoords(global1) );
	ceph::Game::getInstance().addDebugRect( game.convertToScreenCoords(global2) );
	ceph::Game::getInstance().addDebugRect( game.convertToScreenCoords(global3) );
	ceph::Game::getInstance().addDebugRect( game.convertToScreenCoords(global4) );

	addActor(test);

	ship_ = std::make_shared<Ship>(sprite_sheet_);
	auto rect = ceph::Game::getInstance().getLogicalRect();
	ship_->setPosition(rect.x + rect.wd / 2.0f - 100.0f, rect.y + rect.hgt / 2.0f
	);
	ship_->applyAction(
		std::make_shared<ceph::MoveWithWrappingAction>( 120.0f, 0)
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
