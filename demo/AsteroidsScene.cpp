#include <random>
#include "AsteroidsScene.hpp"
#include "cephalopod/texture.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/actions.hpp"
#include "cephalopod/actionconstraints.hpp"
#include "cephalopod/easingactions.hpp"
#include "cephalopod/group.hpp"
#include "cephalopod/sprite.hpp"

std::random_device rd; // obtain a random number from hardware
std::mt19937 eng( rd()  );

void Asteroids::initialize()
{
	sprite_sheet_ = ceph::SpriteSheet::create(
		".\\data\\zarquon.png",
		".\\data\\zarquon.json" 
	);
	setBackgroundColor(ceph::ColorRGB(0, 20, 40));
	
	addActors({
		CreateStarLayer( -256.0f, 0.5f ),
		CreateStarLayer( -128.0f, 0.25f ),
		CreateStarLayer( -64.0f, 0.125f ),
		CreateStarLayer( -32.0f, 0.0625f )
	});
	
	auto ship = ceph::Actor::create<Ship>(sprite_sheet_);

	ship->setPosition(700, 300);
	ship->applyConstraint(
		std::make_shared<ceph::WrapTorroidally>( 40.0f, 40.0f )
	);
	addActor(ship);
	ship->connect(updateEvent, &Ship::update);
}

std::shared_ptr<ceph::Actor> Asteroids::CreateStarLayer(float horz_speed, float alpha)
{
	auto bounds = ceph::Game::getInstance().getLogicalRect();

	std::uniform_int_distribution<> star_distr(1, 7);
	std::uniform_real_distribution<> x_distr(bounds.x, bounds.x2() - horz_speed);
	std::uniform_real_distribution<> y_distr(bounds.y, bounds.y2());

	auto layer = ceph::Actor::create<ceph::Group>();
	for (int i = 0; i < 85; i++) {
		auto star_id = "star_" + std::to_string(star_distr(eng));
		auto star = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, star_id);
		star->setAnchorPt(0.5f, 0.5f);
		star->setPosition( static_cast<float>(x_distr( eng )), static_cast<float>(y_distr( eng )) );
		star->setAlpha(alpha);
		star->applyAction(
			ceph::createMoveByAction(2.0f, horz_speed, 0), true
		);
		star->applyAction(
			ceph::createRotateByAction(1.0f, 1.0f), true
		);
		star->applyConstraint(
			std::make_shared<ceph::WrapTorroidally>(10.0f, 0.0f)
		);
		layer->addChild(star);
	}

	return layer;
}