
#define _USE_MATH_DEFINES
#include <cmath>
#include "cephalopod/types.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/actionconstraints.hpp"
#include "cephalopod/easingactions.hpp"
#include "Ship.hpp"
#include "AsteroidsScene.hpp"

Ship::RotationType Ship::getRotationState() const
{
	if (actions_.hasAction(static_cast<int>(RotationType::Left)))
		return RotationType::Left;
	else if (actions_.hasAction(static_cast<int>(RotationType::Right)))
		return RotationType::Right;
	return RotationType::None;
}

Ship::Ship(const std::shared_ptr<ceph::SpriteSheet>& ss) :
	sprites_(ss),
	is_thruster_on_(false),
	velocity_(0.0f,0.0f)
{
}

void Ship::initialize()
{
	auto main_ship_sprite = ceph::Actor::create<ceph::Sprite>(sprites_, "ship");
	main_ship_sprite->setAnchorPcnt(0.5f, 0.5f);
	addChild(main_ship_sprite);
	auto& key_evt = ceph::Game::getInstance().keyEvent;
	connect(key_evt, &Ship::handleKey);
}

void Ship::update(float dt)
{
	if (is_thruster_on_) {
		auto direction = getDirection();
		velocity_ += dt * (800.0f * direction);
	}

	auto old_position = getPosition();
	moveTo(old_position + dt * velocity_ );
}

void Ship::handleRotationKey(bool key_down, Ship::RotationType direction)
{
	auto& actions = getActions();
	if (!key_down) {
		actions.removeAction(static_cast<int>(direction));
		return;
	}

	bool is_left = direction == RotationType::Left;
	auto rot_type = getRotationState();
	auto opp_direction = is_left ? RotationType::Right : RotationType::Left;

	if (rot_type == direction)
		return;

	if (rot_type == opp_direction)
		actions.removeAction( static_cast<int>(opp_direction) );

	actions.applyAction(
		static_cast<int>(direction),
		ceph::createRotateByAction(1.0f,  8.0f * ((is_left) ? -1.0f : 1.0f))
	);
}
void Ship::handleThrustKey(bool key_down)
{
	is_thruster_on_ = key_down;
}

ceph::Vec2<float> Ship::getDirection() const
{
	float theta = -getRotation();
	return {
		cosf(theta),
		sinf(theta)
	};
}

void Ship::shoot()
{
	auto direction = getDirection();
	auto start_position = getPosition() + 40.0f * direction;
	auto bullet = ceph::Actor::create<ceph::Sprite>(sprites_, "zap");
	bullet->setAnchorPcnt(0.5f, 0.5f);
	bullet->moveTo(start_position);
	bullet->rotateTo(getRotation());

	bullet->getActions().applyAction(
		FLYING_BULLET_ACTION,
		ceph::createActionSequence({
			ceph::createMoveByAction(0.5, 400.0f * direction),
			ceph::createSimultaneousActions({
				ceph::createMoveByAction(0.5, 400.0f * direction),
				ceph::createEasingAction<ceph::EasingFunctionType::Bouncy, ceph::EasingType::Out>(
					ceph::createFadeOutAction(0.5, *bullet)
				)
			})
		})
	);

	auto bkgd = std::static_pointer_cast<Asteroids>(scene_.lock())->getBkgdLayer();
	bullet->getActions().getCompletionSignal(FLYING_BULLET_ACTION).connect(
		*bullet,
		[bullet,bkgd](int i) {
			bkgd->removeChild(bullet);
		}
	);
	
	bkgd->addChild(bullet);
}

void Ship::handleKey(bool is_key_down, ceph::KeyCode key, unsigned char modifiers)
{
	switch (key) {
		case  ceph::KeyCode::Left:
			handleRotationKey(is_key_down, RotationType::Left);
			return;
		case  ceph::KeyCode::Right:
			handleRotationKey(is_key_down, RotationType::Right);
			return;
		case  ceph::KeyCode::Up:
			handleThrustKey(is_key_down);
			return;
		case ceph::KeyCode::Space:
			if (is_key_down)
				shoot();
			return;
	}
}
