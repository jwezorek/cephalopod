/*
#define _USE_MATH_DEFINES
#include <cmath>
#include "actorstate.hpp"
#include "actorimpl.hpp"
#include "util.hpp"
#include "../include/cephalopod/actor.hpp"

ceph::ActorState::ActorState(const ceph::Actor& actor) : 
	alpha_(actor.getAlpha()),
	transform_(actor.impl_->properties)
{
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

std::string ceph::ActorState::getSpriteFrame() const
{
	return sprite_frame_;
}

void ceph::ActorState::setSpriteFrame(const std::string & sprite_frame)
{
	sprite_frame_ = sprite_frame;
}

void ceph::ActorState::setAlpha(float alpha) 
{
	alpha_ = alpha;
}

float ceph::ActorState::getAlpha() const
{
	return alpha_;
}
*/



//	Mat3x3 local_to_global_transform_;
//	std::optional<Mat3x3> transform_;
//	Vec2<float> position_;
//	Vec2<float> scale_;
//	float rotation_;
//	float alpha_;
//	std::string sprite_frame_;


#include "..\include\cephalopod\actorstate.hpp"

namespace
{
	ceph::Mat3x3 GetSpriteMatrix(const ceph::Vec2<int>& sz, const ceph::Vec2<float>& position, const ceph::Vec2<float>& origin_pcnt, const ceph::Vec2<float>& scale, float rotation)
	{
		float origin_x = origin_pcnt.x * sz.x;
		float origin_y = origin_pcnt.y * sz.y;
		float cosine = static_cast<float>(std::cos(rotation));
		float sine = static_cast<float>(std::sin(rotation));
		float sxc = scale.x * cosine;
		float syc = scale.y * cosine;
		float sxs = scale.x * sine;
		float sys = scale.y * sine;
		float tx = -origin_x * sxc - origin_y * sys + position.x;
		float ty = origin_x * sxs - origin_y * syc + position.y;

		return ceph::Mat3x3(
			sxc, sys, tx,
			-sxs, syc, ty,
			0, 0, 1
		);
	}
}

ceph::ActorState::ActorState() :
	position_(0, 0),
	scale_(1, 1),
	rotation_(0),
	alpha_(1.0f),
	origin_pcnt_(0.5f,0.5f),
	transform_(std::nullopt)
{}

void ceph::ActorState::translate(const ceph::Vec2<float>& v)
{
	position_ += v;
	transform_ = std::nullopt;
}

void ceph::ActorState::translate(float x, float y)
{	
	position_.x += x;
	position_.y += y;
	transform_ = std::nullopt;
}

void ceph::ActorState::rotate(float theta)
{
	rotation_ += theta;
	transform_ = std::nullopt;
}

ceph::Vec2<float> ceph::ActorState::getScale() const
{
	return scale_;
}

void ceph::ActorState::setScale(const Vec2<float>& s)
{
	scale_ = s;
	transform_ = std::nullopt;
}

void ceph::ActorState::setAlpha(float alpha)
{
	alpha_ = alpha;
}

float ceph::ActorState::getAlpha() const
{
	return alpha_;
}

ceph::Vec2<float> ceph::ActorState::getGlobalPosition() const
{
	//TODO
	return ceph::Vec2<float>(0, 0);
}

void ceph::ActorState::setGlobalPosition(const Vec2<float>& pt)
{
	//TODO
}

void ceph::ActorState::setPosition(const ceph::Vec2<float>& v)
{
	position_ = v;
	transform_ = std::nullopt;
}

void ceph::ActorState::setPosition(float x, float y)
{
	setPosition(ceph::Vec2<float>(x, y));
}

ceph::Vec2<float> ceph::ActorState::getPosition() const
{
	return position_;
}

float ceph::ActorState::getRotation() const
{
	return rotation_;
}

std::string ceph::ActorState::getSpriteFrame() const
{
	return sprite_frame_;
}

void ceph::ActorState::setSpriteFrame(const std::string& sprite_frame, ceph::Vec2<int> sz)
{
	sprite_frame_ = sprite_frame;
	frame_sz_ = sz;
}

void ceph::ActorState::setAnchorPcnt(const ceph::Vec2<float>& v)
{
	origin_pcnt_ = v;
	transform_ = std::nullopt;
}

ceph::Vec2<float> ceph::ActorState::getAnchorPcnt() const
{
	return origin_pcnt_;
}

ceph::Vec2<float> ceph::ActorState::getAnchorPt() const
{
	return ceph::Vec2<float>(
		origin_pcnt_.x * frame_sz_.x,
		origin_pcnt_.y * frame_sz_.y
	);
}

void ceph::ActorState::setAnchorPcnt(float x, float y)
{
	setAnchorPcnt(
		ceph::Vec2<float>(x,y)
	);
}

void ceph::ActorState::setRotation(float theta)
{
	rotation_ = theta;
	transform_ = std::nullopt;
}

ceph::Vec2<int> ceph::ActorState::getFrameSize() const
{
	return frame_sz_;
}

ceph::Mat3x3 ceph::ActorState::getTransformationMatrix() const
{
	if (!transform_.has_value())
		transform_ = GetSpriteMatrix(frame_sz_, position_, origin_pcnt_, scale_, rotation_);
	return transform_.value();
}