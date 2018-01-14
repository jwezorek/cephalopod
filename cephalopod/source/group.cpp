#include "../include/cephalopod/group.hpp"
#include "actorimpl.hpp"
#include "spriteimpl.hpp"

ceph::Group::Group() : impl_(std::make_unique<SpriteImpl>())
{
}

ceph::Vec2D<float> ceph::Group::getGlobalPosition() const
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

	auto sp = trans.transformPoint(sf::Vector2<float>(p.x, p.y));
	return ceph::Vec2D<float>(sp.x, sp.y);
}

void ceph::Group::setGlobalPosition(const Vec2D<float>& pt_global)
{
	if (!hasParent()) {
		setPosition(pt_global);
	}
	else {
		auto parent_pos = parent_.lock()->getGlobalPosition();
		setPosition(pt_global.x - parent_pos.x, pt_global.y - parent_pos.y);
	}
}

ceph::Rect<float> ceph::Group::getLocalBounds() const
{
	//TODO
	throw std::runtime_error("ceph::Group::getLocalBounds() not implemented.");
}

ceph::Rect<float> ceph::Group::getGlobalBounds() const
{
	return getTotalGlobalBounds();
}