#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/game.hpp"
#include "gameimpl.hpp"
#include "textureimpl.hpp"
#include "spriteimpl.hpp"
#include "drawingcontext.hpp"
#include "actorimpl.hpp"
#include "SFML/Graphics.hpp"

/*--------------------------------------------------------------------------------*/

sf::IntRect ToSfmlRect(const ceph::Rect<int>& r)
{
	return sf::IntRect(r.x, r.y, r.wd, r.hgt);
}

ceph::SpriteImpl::SpriteImpl() :
	sfml_sprite_()
{
}

ceph::SpriteImpl::SpriteImpl(const std::shared_ptr<ceph::Texture>& texture) :
	sfml_sprite_(texture->impl_->sfml_impl_)
{
}

ceph::SpriteImpl::SpriteImpl(const std::shared_ptr<ceph::Texture>& texture, const ceph::Rect<int>& r) :
	sfml_sprite_(texture->impl_->sfml_impl_, ToSfmlRect(r) )
{
}

ceph::SpriteImpl::SpriteImpl(const ceph::SpriteFrame& frame) :
	sfml_sprite_( frame.getTexture()->impl_->sfml_impl_, ToSfmlRect(frame.getRect()) )
{
}

/*--------------------------------------------------------------------------------*/

ceph::Sprite::Sprite(const std::shared_ptr<Texture>& texture):
	impl_(std::make_unique<SpriteImpl>(texture)),
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
	impl_ = std::make_unique<SpriteImpl>(sprite_frame_);
	ceph::Actor::impl_->frame_sz = sprite_frame_.getRect().getSize();
}

void ceph::Sprite::drawThis(DrawingContext& dc) const
{
	auto color = impl_->sfml_sprite_.getColor();
	sf::Uint8 alpha_byte = static_cast<sf::Uint8>(std::round(dc.alpha * 255.0));
	impl_->sfml_sprite_.setColor(sf::Color(255, 255, 255, alpha_byte));
	dc.target.draw( impl_->sfml_sprite_, dc.transform);
	impl_->sfml_sprite_.setColor(color);
}

ceph::Vec2D<float> ceph::Sprite::getGlobalPosition() const
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
	return ceph::Vec2D<float>(sp.x, sp.y);
}

void  ceph::Sprite::setGlobalPosition(const ceph::Vec2D<float>& pt_global)
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

ceph::Sprite::~Sprite()
{
}
