#define _USE_MATH_DEFINES
#include <cmath>
#include "actorstate.hpp"
#include "actorimpl.hpp"
#include "util.hpp"
#include "../include/cephalopod/actor.hpp"

ceph::ActorState::ActorState(const ceph::Actor& actor)
{
	transform_ = actor.impl_->properties;
	auto parent = actor.getParent();
	sf::Transform trans;
	while (!parent.expired())
	{
		auto& parent_actor_impl = *(parent.lock()->impl_);
		auto parentTransform = parent_actor_impl.properties.getTransform();
		trans = parentTransform.combine(trans);
		parent = parent.lock()->getParent();
	}
	local_to_global_transform_ = trans;
}

void ceph::ActorState::translate(float x, float y)
{
	transform_.move(x, y);
}

void ceph::ActorState::translate(const ceph::Vec2D<float>& v)
{
	translate(v.x, v.y);
}

void ceph::ActorState::rotate(float theta)
{
	transform_.rotate(radiansToDegrees(theta));
}

sf::Vector2f ceph::ActorState::getGlobalPosition() const
{
	return local_to_global_transform_.transformPoint( getPosition() );
}

void ceph::ActorState::setGlobalPosition(const sf::Vector2f& pt)
{
	auto globalToLocal = local_to_global_transform_.getInverse();
	transform_.setPosition(globalToLocal.transformPoint(pt));
}

sf::Vector2f ceph::ActorState::getPosition() const
{
	return transform_.getPosition();
}

float ceph::ActorState::getRotation() const
{
	return transform_.getRotation();
}

sf::Vector2f ceph::ActorState::getScale() const
{
	return  transform_.getScale();
}
