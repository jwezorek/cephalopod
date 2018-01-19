#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/scene.hpp"
#include "SFML/Graphics.hpp"
#include "drawingcontext.hpp"
#include "actorimpl.hpp"
#include "actorstate.hpp"
#include "util.hpp"

ceph::Actor::Actor() :
	impl_(std::make_unique<ceph::ActorImpl>(*this))
{
}

void ceph::Actor::addChild(const std::shared_ptr<ceph::Actor>& child)
{
	child->parent_ = shared_from_this();
	children_.push_back(child);
	if (isInScene()) {
		child->attachToScene(scene_.lock());
		if (child->hasActions())
			child->runActions();
	}
}

void ceph::Actor::addChildren(std::initializer_list<std::shared_ptr<Actor>> children)
{
	for (const auto& child : children)
		addChild(child);
}

void ceph::Actor::removeChild(const std::shared_ptr<ceph::Actor>& actor)
{
	auto i = std::find(children_.begin(), children_.end(), actor);
	if (i == children_.end())
		return;

	(*i)->parent_ = std::weak_ptr<Actor>();
	(*i)->detachFromScene();

	children_.erase(i);
}

void ceph::Actor::detach()
{
	if (hasParent())
		parent_.lock()->removeChild(shared_from_this());
	else if (isInScene())
		scene_.lock()->removeActor(shared_from_this());
}

void ceph::Actor::detachFromScene()
{
	scene_ = std::weak_ptr<ceph::Scene>();
	for (auto child : children_)
		child->detachFromScene();
}

void ceph::Actor::attachToScene(const std::shared_ptr<ceph::Scene>& scene)
{
	scene_ = scene;
	for (auto child : children_)
		child->attachToScene(scene);
}

bool ceph::Actor::isInScene() const
{
	return !scene_.expired();
}

bool ceph::Actor::hasParent() const
{
	return !parent_.expired();
}

bool ceph::Actor::isInSceneTopLevel() const
{
	return isInScene() && !hasParent();
}

ceph::ActionPlayer& ceph::Actor::getActions() const
{
	return impl_->actions;
}

void ceph::Actor::runActions()
{
	if (hasActions())
		impl_->actions.run();
	for (auto& child : children_)
		child->runActions();
}

bool ceph::Actor::hasActions() const
{
	if (impl_->actions.hasActions())
		return true;
	for (const auto& child : children_)
		if (child->hasActions())
			return true;
	return false;
}

std::weak_ptr<ceph::Actor> ceph::Actor::getParent() const
{
	return parent_;
}

std::weak_ptr<ceph::Actor> ceph::Actor::getTopLevelParent() const
{
	std::weak_ptr<ceph::Actor> parent = parent_;
	while (!parent.expired())
		parent = parent.lock()->getParent();
	return parent;
}

std::weak_ptr<ceph::Scene> ceph::Actor::getScene() const
{
	return scene_;
}

float ceph::Actor::getAlpha() const
{
	return impl_->alpha;
}

void ceph::Actor::setAlpha(float alpha)
{
	impl_->alpha = alpha;
}

float ceph::Actor::getRotation() const
{
	return ceph::degreesToRadians(impl_->properties.getRotation());
}

void ceph::Actor::setRotation(float radians)
{
	impl_->properties.setRotation(ceph::radiansToDegrees(radians));
}

float ceph::Actor::getRotationDegrees() const
{
	return impl_->properties.getRotation();
}

void ceph::Actor::setRotationDegrees(float degrees)
{
	impl_->properties.setRotation(degrees);
}

float ceph::Actor::getScale() const
{
	return impl_->properties.getScale().x;
}

void ceph::Actor::setScale(float scale)
{
	impl_->properties.scale(scale, scale);
}

ceph::Vec2D<float> ceph::Actor::getPosition() const
{
	auto origin = impl_->properties.getPosition();
	return ceph::Vec2D<float>(origin.x, origin.y);
}

void ceph::Actor::setPosition(const ceph::Vec2D<float>& pt)
{
	if (! impl_->actions.isRunning()) {
		impl_->properties.setPosition(sf::Vector2f(pt.x, pt.y));
	} else {
		auto diff = pt - getPosition();
		impl_->properties.setPosition(sf::Vector2f(pt.x, pt.y));
		impl_->actions.translateCacheState(diff);
	}
}

void ceph::Actor::setPosition(float x, float y)
{
	setPosition(ceph::Vec2D<float>(x, y));
}

ceph::Vec2D<float> ceph::Actor::getAnchorPt() const
{
	auto origin = impl_->properties.getOrigin();
	return ceph::Vec2D<float>(origin.x / impl_->frame_sz.wd, origin.y / impl_->frame_sz.hgt);
}

void ceph::Actor::setAnchorPt(const ceph::Vec2D<float>& pt)
{
	setAnchorPt(pt.x, pt.y);
}

void ceph::Actor::setAnchorPt(float x, float y)
{
	impl_->anchor = ceph::Vec2D<float>(x,y);
	impl_->properties.setOrigin(
		x * static_cast<float>(impl_->frame_sz.wd),
		y * static_cast<float>(impl_->frame_sz.hgt)
	);
}

ceph::Rect<float> ceph::Actor::getTotalGlobalBounds() const
{
	auto bounds = getGlobalBounds();
	for (const auto& child : children_)
		bounds.unionWith(child->getGlobalBounds());
	return bounds;
}

void ceph::Actor::draw(DrawingContext& dcParent) const 
{
	DrawingContext dc(
		dcParent.target, 
		dcParent.transform * impl_->properties.getTransform(), 
		dcParent.alpha * impl_->alpha
	);
	drawThis(dc);
	dc.transform *= sf::Transform().translate(impl_->properties.getOrigin());
	for (const auto& child : children_) {
		child->draw(dc);
	}
}

ceph::Actor::~Actor()
{
}
