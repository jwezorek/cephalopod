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
	setBackground( std::make_shared<ceph::Texture>(".\\data\\escher_square.png") );

	ship_ = sprite_sheet_->getSprite("diamonds");
	auto origin = ship_->getAnchorPt();
	ship_->setRotation(3.14159265358979323846f / 4.0f);

	auto diamond1 = sprite_sheet_->getSprite("red_diamond");
	diamond1->setPosition(ceph::Point<float>(36, 36));

	auto diamond2 = sprite_sheet_->getSprite("red_diamond");
	diamond2->setPosition(ceph::Point<float>(192, 36));

	ship_->AddChild(diamond1);
	ship_->AddChild(diamond2);

	auto rect = ceph::Game::getInstance().getLogicalRect();
	auto squ1 = sprite_sheet_->getSprite("little_grid");
	auto squ2 = sprite_sheet_->getSprite("little_grid");

	auto loc = rect.getLocation();
	squ1->setPosition( loc );
	squ2->setPosition(rect.getLocation().x + rect.getSize().wd - 25, rect.getLocation().y + rect.getSize().hgt - 25);

	updateEvent.connect(*ship_, 
		[=](float dt) {
			updateShip(dt);
		}
	);

	this->addActor(ship_);
	this->addActor(squ1);
	this->addActor(squ2);

	auto blue_square = sprite_sheet_->getSprite("blue_square");
	blue_square->setAnchorPt(0.5f, 0.5f);
	
	std::vector<std::shared_ptr<ceph::Sprite>> green = {
		sprite_sheet_->getSprite("green_square"),
		sprite_sheet_->getSprite("green_square"),
		sprite_sheet_->getSprite("green_square"),
		sprite_sheet_->getSprite("green_square")
	};
	float theta = 0;
	for (auto& sprite : green) {
		float s = 1.0f / 3.0f;
		sprite->setScale(s);
		sprite->setAnchorPt(0.5f, 0.5f);
		float x = s * 50.0f * cos(theta);
		float y = s * 50.0f * sin(theta);
		sprite->setPosition(x, y);
		blue_square->AddChild(sprite);
		theta += M_PI / 2.0f;
	}

	blue_square->setPosition(200, 200);
	blue_square->setAlpha(0.25f);

	this->addActor(blue_square);
	
	updateEvent.connect(*blue_square,
		[=](float dt) {
			float theta = blue_square->getRotation();
			theta += 2*dt;
			blue_square->setRotation(theta);
		}
	);
}

void Asteroids::updateShip(float dt)
{
	auto pt = ship_->getPosition();
	pt.x += dt * 100.0f;
	pt.y += dt * 100.0f;

	auto sz = ceph::Game::getInstance().getLogicalSize();
	if (pt.y > sz.hgt)
		pt.y = 0.0f;

	if (pt.x > sz.wd)
		pt.x = 0.0f;

	ship_->setPosition(pt);
}
