#define _USE_MATH_DEFINES
#include <cmath>
#include "Ship.hpp"
#include "cephalopod/types.hpp"
#include "cephalopod/game.hpp"

Ship::RotationType Ship::getRotationState() const
{
	if (hasAction(static_cast<int>(RotationType::Left)))
		return RotationType::Left;
	else if (hasAction(static_cast<int>(RotationType::Right)))
		return RotationType::Right;
	return RotationType::None;
}

Ship::Ship(const std::shared_ptr<ceph::SpriteSheet>& ss) :
	ceph::Sprite(ss, "ship" ),
	is_thruster_on_(false),
	velocity_(0.0f,0.0f)
{
	ceph::Vec2D<float> pt(0.5f, 0.5f);
	setAnchorPt(pt);

	auto& key_evt = ceph::Game::getInstance().keyEvent;
	connect( key_evt, &Ship::handleKey);
}

void Ship::update(float dt)
{
	if (is_thruster_on_) {
		auto direction = getDirection();
		velocity_ += dt * (800.0f * direction);
	}

	auto old_position = getPosition();
	setPosition(old_position + dt * velocity_ );

	enforceConstraints();
}

void Ship::handleRotationKey(bool key_down, Ship::RotationType direction)
{
	bool is_left = direction == RotationType::Left;
	auto rot_type = getRotationState();
	auto opp_direction = is_left ? RotationType::Right : RotationType::Left;
	if (key_down) {
		if (rot_type == direction)
			return;
		if (rot_type == opp_direction)
			removeAction( static_cast<int>(opp_direction) );
		applyAction(
			static_cast<int>(direction),
			ceph::createRotateByAction(1.0f,  8.0f * ((is_left) ? -1.0f : 1.0f))
		);
	}
	else {
		removeAction(static_cast<int>(direction));
	}
}


void Ship::handleThrustKey(bool key_down)
{
	is_thruster_on_ = key_down;
}

ceph::Vec2D<float> Ship::getDirection() const
{
	auto theta = getRotation() - M_PI / 2.0f;
	return {
		cosf(theta),
		sinf(theta)
	};
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
	}
}
