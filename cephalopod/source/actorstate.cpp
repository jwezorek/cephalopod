#include "..\include\cephalopod\actorstate.hpp"
#include "..\include\cephalopod\spritesheet.hpp"
#include "util.hpp"

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
	sprite_sheet_(nullptr),
	position_(0, 0),
	scale_(1, 1),
	rotation_(0),
	alpha_(1.0f),
	origin_pcnt_(0.5f,0.5f),
	transform_(std::nullopt)
{}

void ceph::ActorState::setSpriteSheet(const std::shared_ptr<const SpriteSheet>& sheet)
{
	sprite_sheet_ = sheet;
}

std::shared_ptr<const ceph::SpriteSheet> ceph::ActorState::getSpriteSheet() const
{
	return sprite_sheet_;
}

void ceph::ActorState::moveBy(const ceph::Vec2<float>& v)
{
	position_ += v;
	transform_ = std::nullopt;
}

void ceph::ActorState::rotateBy(float theta)
{
	rotation_ = normalizeAngle(rotation_ + theta);
	transform_ = std::nullopt;
}

ceph::Vec2<float> ceph::ActorState::getScale() const
{
	return scale_;
}

void ceph::ActorState::changeScaleBy(const Vec2<float>& s)
{
	scale_ += s;
	transform_ = std::nullopt;
}

void ceph::ActorState::changeAlphaBy(float alpha_delta)
{
	alpha_ = ceph::clampValue(alpha_ + alpha_delta, 0, 1);
}

float ceph::ActorState::getAlpha() const
{
	return alpha_;
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

void ceph::ActorState::setSpriteFrame(const std::string& sprite_frame)
{
	sprite_frame_ = sprite_frame;
	frame_sz_ = sprite_sheet_->getFrameSize(sprite_frame);
}

ceph::Vec2<int> ceph::ActorState::getSize() const
{
	return frame_sz_;
}

ceph::Rect<int> ceph::ActorState::getRect() const
{
	return sprite_sheet_->getFrame(sprite_frame_);
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

ceph::Mat3x3 ceph::ActorState::getTransformationMatrix() const
{
	if (!transform_.has_value())
		transform_ = GetSpriteMatrix(frame_sz_, position_, origin_pcnt_, scale_, rotation_);
	return transform_.value();
}