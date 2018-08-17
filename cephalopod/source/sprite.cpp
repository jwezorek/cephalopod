#include <memory>
#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/game.hpp"
#include "gameimpl.hpp"
#include "../include/cephalopod/drawingcontext.hpp"

ceph::Sprite::Sprite(const std::shared_ptr<Texture>& texture):
	sprite_frame_(texture)
{
}

ceph::Sprite::Sprite(const std::shared_ptr<const SpriteSheet>& sheet, const std::string& frame_name) :
	sprite_sheet_(sheet)
{
	setFrame(frame_name);
}

void ceph::Sprite::setFrame(const std::string& frame_name)
{
	sprite_frame_ = sprite_sheet_->getSpriteFrame(frame_name);
	state_.setSpriteFrame(frame_name, sprite_frame_.getSize());
}

ceph::Vec2<float> ceph::Sprite::getGlobalPosition() const
{
	return ceph::Vec2<float>();
}

void ceph::Sprite::setGlobalPosition(const ceph::Vec2<float>& pt_global)
{
}

ceph::Rect<float> ceph::Sprite::getLocalBounds() const
{
	auto sprite_sz = sprite_frame_.getSize();
	auto transformation = state_.getTransformationMatrix() * 
		ceph::Mat3x3().scale(static_cast<float>(sprite_sz.x), static_cast<float>(sprite_sz.y));
	return transformation.apply( ceph::Rect<float>(0, 0, 1, 1) );
}

ceph::Rect<float> ceph::Sprite::getGlobalBounds() const
{
	auto bounds = getLocalBounds();
	auto parent = getParent().lock();
	ceph::Mat3x3 transform;
	while (parent) {
		transform = parent->state_.getTransformationMatrix() * ceph::Mat3x3().translate(parent->getAnchorPt()) * transform;
		parent = parent->getParent().lock();
	}

	return transform.apply(bounds);
}

void ceph::Sprite::drawThis(DrawingContext& dc) const
{
	auto curr_tex = dc.graphics.GetCurrentTexture();
	auto sprite_sheet_tex = sprite_sheet_->getTexture();

	if (curr_tex.get() != sprite_sheet_tex.get())
		dc.graphics.SetCurrentTexture(sprite_sheet_tex);

	auto sprite_sz = sprite_frame_.getSize();
	auto matrix = dc.transformation * ceph::Mat3x3().scale(static_cast<float>(sprite_sz.x), static_cast<float>(sprite_sz.y));
	dc.graphics.Blit(matrix, sprite_frame_.getRect(), dc.alpha);
}

//TODO
/*
ceph::Vec2<float> ceph::Sprite::getGlobalPosition() const
{
	auto p = getPosition();
	auto& actor_impl = *(static_cast<const Actor*>(this)->impl_);
	auto trans = actor_impl.properties.getTransform();
	auto parent = getParent();
	//TODO: make the following be a subroutine.
	while (!parent.expired())
	{
		auto& parent_actor_impl = *(parent.lock()->impl_);
		auto parentTransform = parent_actor_impl.properties.getTransform();
		trans = parentTransform.combine(trans);;
		parent = parent.lock()->getParent();
	}

	auto sp = trans.transformPoint( sf::Vector2<float>(p.x,p.y) );
	return ceph::Vec2<float>(sp.x, sp.y);
}

void  ceph::Sprite::setGlobalPosition(const ceph::Vec2<float>& pt_global)
{
	if (!hasParent()) {
		setPosition(pt_global);
	} else {
		auto parent_pos = parent_.lock()->getGlobalPosition();
		setPosition(pt_global.x - parent_pos.x, pt_global.y - parent_pos.y);
	}
}

ceph::Rect<float> ceph::Sprite::getLocalBounds() const
{
	auto bounds = impl_->sfml_sprite_.getLocalBounds();
	auto& actor_impl = *(static_cast<const Actor*>(this)->impl_);
	bounds = actor_impl.properties.getTransform().transformRect(bounds);
	return ceph::Rect<float>(bounds.left, bounds.top, bounds.width, bounds.height);
}

ceph::Rect<float> ceph::Sprite::getGlobalBounds() const
{
	auto bounds = impl_->sfml_sprite_.getLocalBounds();
	auto& actor_impl = *(static_cast<const Actor*>(this)->impl_);
	auto trans = actor_impl.properties.getTransform();
	auto parent = getParent();

	while (!parent.expired())
	{
		auto& parent_actor_impl = *(parent.lock()->impl_);
		auto parentTransform = parent_actor_impl.properties.getTransform();
		trans = parentTransform.combine(trans);;
		parent = parent.lock()->getParent();
	}

	bounds = trans.transformRect(bounds);
	return ceph::Rect<float>(bounds.left, bounds.top, bounds.width, bounds.height);
}
*/

ceph::Sprite::~Sprite()
{
}
