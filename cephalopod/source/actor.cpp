#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/Game.hpp"
#include "../include/cephalopod/Scene.hpp"
#include "../include/cephalopod/actorstate.hpp"
#include "util.hpp"

ceph::Actor::Actor() : actions_(*this)
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

	auto scene = actor->getScene().lock();

	(*i)->parent_ = std::weak_ptr<Actor>();
	(*i)->detachFromScene();
	children_.erase(i);

	if (scene != nullptr || scene == Game::getInstance().getActiveScene())
		scene->dropped_actors_.push_back(actor);
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

ceph::ActorState  ceph::Actor::getState() const
{
	return state_;
}

ceph::ActionPlayer& ceph::Actor::getActions()
{
	return actions_;
}

void ceph::Actor::runActions()
{
	if (hasActions())
		actions_.run();
	for (auto& child : children_)
		child->runActions();
}

ceph::Mat3x3 ceph::Actor::getLocalToGlobalTransform() const
{
	ceph::Mat3x3 transform;
	auto parent = getParent().lock();
	while (parent) {
		transform = parent->state_.getTransformationMatrix() * ceph::Mat3x3().translate(parent->getAnchorPt()) * transform;
		parent = parent->getParent().lock();
	}
	return transform;
}

ceph::Vec2<float> ceph::Actor::getGlobalPosition() const
{
	return getLocalToGlobalTransform().apply(state_.getPosition());
}

void ceph::Actor::setGlobalPosition(const ceph::Vec2<float>& pt_global)
{
	setPosition(
		getGlobalToLocalTransform().apply(pt_global)
	);
}

ceph::Rect<float> ceph::Actor::getLocalBounds() const
{
	auto sprite_sz = state_.getSize();
	auto transformation = state_.getTransformationMatrix() *
		ceph::Mat3x3().scale(static_cast<float>(sprite_sz.x), static_cast<float>(sprite_sz.y));
	return transformation.apply(ceph::Rect<float>(0, 0, 1, 1));
}

ceph::Rect<float> ceph::Actor::getGlobalBounds() const
{
	return getLocalToGlobalTransform().apply(
		getLocalBounds()
	);
}

ceph::Mat3x3 ceph::Actor::getGlobalToLocalTransform() const
{
	return getLocalToGlobalTransform().getInverse().value();
}

bool ceph::Actor::hasActions() const
{
	if (actions_.hasActions())
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
	return state_.getAlpha();
}

void ceph::Actor::setAlpha(float alpha)
{
	alpha = (alpha > 1.0f) ? 1.0f : alpha;
	alpha = (alpha < 0.0f) ? 0.0f : alpha;
	state_.setAlpha(alpha);
}

float ceph::Actor::getRotation() const
{
	return state_.getRotation();
}

void ceph::Actor::setRotation(float radians)
{
	state_.setRotation(radians);
}

ceph::Vec2<float> ceph::Actor::getScale() const
{
	return state_.getScale();
}

void ceph::Actor::setScale(float scale)
{
	state_.setScale(ceph::Vec2<float>(scale, scale));
}

void ceph::Actor::setScale(const ceph::Vec2<float>& s)
{
	state_.setScale(s);
}

ceph::Vec2<float> ceph::Actor::getPosition() const
{
	return state_.getPosition();
}

void ceph::Actor::setPosition(const ceph::Vec2<float>& pt)
{
	//if (! impl_->actions.isRunning()) {
	state_.setPosition(pt);
	//} else {
	//	auto diff = pt - getPosition();
	//	impl_->properties.setPosition(sf::Vector2f(pt.x, pt.y));
	//	impl_->actions.translateCacheState(diff);
	//}
}

void ceph::Actor::setPosition(float x, float y)
{
	setPosition(ceph::Vec2<float>(x, y));
}

ceph::Vec2<float> ceph::Actor::getAnchorPt() const
{
	return state_.getAnchorPt();
}

ceph::Vec2<float> ceph::Actor::getAnchorPcnt() const
{
	return state_.getAnchorPcnt();
}

void ceph::Actor::setAnchorPcnt(const ceph::Vec2<float>& pt)
{
	state_.setAnchorPcnt(pt.x, pt.y);
}

void ceph::Actor::setAnchorPcnt(float x, float y)
{
	state_.setAnchorPcnt(ceph::Vec2<float>(x, y));
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
		dcParent.graphics,
		dcParent.transformation * state_.getTransformationMatrix(),
		dcParent.alpha * state_.getAlpha() 
	);
	drawThis(dc);
	dc.transformation *= ceph::Mat3x3().translate(state_.getAnchorPt());
	for (const auto& child : children_)
		child->draw(dc);
}

ceph::Actor::~Actor()
{
}
