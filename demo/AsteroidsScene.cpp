#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include "AsteroidsScene.hpp"
#include "cephalopod/texture.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/actions.hpp"
#include "cephalopod/actionconstraints.hpp"
#include "cephalopod/easingactions.hpp"
#include "cephalopod/group.hpp"
#include "cephalopod/sprite.hpp"
#include "Ship.hpp"

std::random_device rd; // obtain a random number from hardware
std::mt19937 eng( rd()  );

void Asteroids::initialize()
{
	sprite_sheet_ = ceph::SpriteSheet::create(
		".\\data\\zarquon.png",
		".\\data\\zarquon.json" 
	);
	setBackgroundColor(ceph::ColorRGB(10, 30, 60));
	
	addActors({
		CreateStarLayer( -256.0f, 0.5f ),
		CreateStarLayer( -128.0f, 0.25f ),
		CreateStarLayer( -64.0f, 0.125f ),
		CreateStarLayer( -32.0f, 0.0625f )
	});

	/*
	addActor(
		bkgd_layer_ = ceph::Actor::create<ceph::Group>()
	);
	
	auto ship = ceph::Actor::create<Ship>(sprite_sheet_);

	ship->moveTo(700, 300);
	ship->applyConstraint(
		std::make_shared<ceph::WrapTorroidally>( 40.0f, 40.0f )
	);
	addActor(ship);
	ship->connect(updateEvent, &Ship::update);
	*/

	for (int i = 0; i < 5; i++) {
		auto asteroid = createAsteroid();
		addActor(asteroid);
	}
}

std::shared_ptr<ceph::Group> Asteroids::getBkgdLayer() const
{
	return bkgd_layer_;
}

std::shared_ptr<ceph::Actor> Asteroids::CreateStarLayer(float horz_speed, float alpha)
{
	auto bounds = ceph::Game::getInstance().getLogicalRect();

	std::uniform_int_distribution<> star_distr(1, 7);
	std::uniform_real_distribution<> x_distr(bounds.x, bounds.x2() - horz_speed);
	std::uniform_real_distribution<> y_distr(bounds.y, bounds.y2());

	auto layer = ceph::Actor::create<ceph::Group>();

	for (int i = 0; i < 45; i++) {
		auto star_id = "star_" + std::to_string(star_distr(eng));
		auto star = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, star_id);
		auto& star_actions = star->getActions();
		star->setAnchorPcnt(0.5f, 0.5f);
		star->moveTo( static_cast<float>(x_distr( eng )), static_cast<float>(y_distr( eng )) );
		star->setAlphaTo(alpha);
		star_actions.applyAction(
			ceph::createMoveByAction(2.0f, horz_speed, 0), true
		);
		star_actions.applyAction(
			ceph::createRotateByAction(1.0f, 1.0f), true
		);
		star->applyConstraint(
			std::make_shared<ceph::WrapTorroidally>(10.0f, 0.0f)
		);
		layer->addChild(star);
	}

	return layer;
}

std::shared_ptr<ceph::Sprite> Asteroids::createAsteroid()
{
	static const std::vector<std::tuple<std::string, int>> asteroid_sprites = {
		{"red_asteroid", 32}, {"blue_asteroid", 24}, {"green_asteroid", 24}
	};

	const auto& sprite_frame_info = asteroid_sprites[std::uniform_int_distribution<>(0, asteroid_sprites.size()-1)(eng)];
	auto frame_prefix = std::get<0>(sprite_frame_info);
	auto num_frames = std::get<1>(sprite_frame_info);

	auto bounds = ceph::Game::getInstance().getLogicalRect();
	std::uniform_real_distribution<> x_distr(bounds.x, bounds.x2());
	std::uniform_real_distribution<> y_distr(bounds.y, bounds.y2());
	std::uniform_real_distribution<> rot_distr(0.0f, 2.0f*M_PI);

	auto asteroid = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, frame_prefix + "_0");
	asteroid->moveTo(x_distr(eng), y_distr(eng));

	auto theta = rot_distr(eng);
	asteroid->getActions().applyAction(
		ceph::createMoveByAction(1.0f, 100.0f * ceph::Vec2<float>(cosf(theta), sinf(theta))),
		true
	);
	asteroid->rotateTo(rot_distr(eng));

	std::vector<std::string> frames;
	for (int i = 0; i < num_frames; i++)
		frames.push_back(frame_prefix + "_" + std::to_string(i));

	asteroid->getActions().applyAction(
		ceph::createAnimationAction(0.05f, frames.begin(), frames.end()),
		true
	);
	
	asteroid->applyConstraint(
		std::make_shared<ceph::WrapTorroidally>(100.0f, 100.0f)
	);

	//asteroid->setScale(0.25f);

	addActor(asteroid);
	return asteroid;
}

/*
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include "AsteroidsScene.hpp"
#include "cephalopod/texture.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/sprite.hpp"

std::random_device rd; 
std::mt19937 eng(rd());

std::shared_ptr<ceph::Sprite> ship;

void Update(float dt)
{
	
	auto pos = ship->getPosition();

	pos += dt * ceph::Vec2<float>(100, 100);

	if (pos.x > 1024)
		pos.x -= 1024;

	if (pos.y > 768)
		pos.y -= 768;

	ship->moveTo(pos);
	ship->rotateBy(2 * dt);
	
}

void Asteroids::initialize()
{
	sprite_sheet_ = ceph::SpriteSheet::create(
		".\\data\\zarquon.png",
		".\\data\\zarquon.json"
	);
	setBackgroundColor(ceph::ColorRGB(10, 30, 60));

	ship = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, "ship");
	ship->connect(updateEvent, std::function<void(float)>(Update));

	ship->moveTo(300, 300);
	ship->setAnchorPcnt(0.5f, 0.5f);
	addActor(ship);

	auto test = ceph::Actor::create<ceph::Sprite>( sprite_sheet_, "test3" );
	test->moveTo(300, 300);
	test->setAnchorPcnt(0.5f, 0.5f);
	addActor(test);

	auto child = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, "test2");
	child->setAnchorPcnt(0.5f, 0.5f);
	child->moveTo(20, 20);
	//child->setRotation(0.78539816339f);
	//addActor(child);
	test->addChild(child);

	auto grandChild = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, "test2");
	grandChild->setAnchorPcnt(0.5f, 0.5f);
	grandChild->moveTo(0, 0);
	grandChild->setScaleTo( 0.5f );
	child->addChild(grandChild);

	auto action = ceph::createMoveByAction( 1.0f, ceph::Vec2<float>(300.0f, 0) );

	auto testGP = child->getGlobalPosition();

	test->getActions().applyAction( action, true );
	test->applyConstraint(std::make_shared<ceph::WrapTorroidally>(80.0f, 80.0f));

	auto bounds = grandChild->getGlobalBounds();
	int aaa;
	aaa = 5;
}
*/



