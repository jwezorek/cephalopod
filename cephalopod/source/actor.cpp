#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/Game.hpp"
#include "../include/cephalopod/Scene.hpp"
#include "../include/cephalopod/actorstate.hpp"
#include "util.hpp"

ceph::Actor::Actor() : actions_(*this), scene_(nullptr)
{
}

void ceph::Actor::addChild(const std::shared_ptr<ceph::Actor>& child)
{
	child->parent_ = shared_from_this();
	children_.push_back(child);
	if (isInScene()) {
		child->attachToScene(*scene_);
		if (child->hasActions())
			child->runActions();
	}
}

void ceph::Actor::addChildren(std::initializer_list<std::shared_ptr<Actor>> children)
{
	for (const auto& child : children)
		addChild(child);
}

void ceph::Actor::removeChild(std::vector<std::shared_ptr<Actor>>::iterator i)
{
	auto actor = *i;

	(*i)->parent_ = std::weak_ptr<Actor>();
	(*i)->detachFromScene();
	children_.erase(i);

	if (isInScene())
	{
		auto& scene = getScene();
		if (&scene == &(Game::getInstance().getActiveScene()))
			scene.dropped_actors_.push_back(actor);
	}
}

void ceph::Actor::removeChild(const std::shared_ptr<ceph::Actor>& actor)
{
	auto i = std::find(children_.begin(), children_.end(), actor);
	if (i == children_.end())
		return;

	removeChild(i);
}

void ceph::Actor::removeAllChildren()
{
	while (!children_.empty())
		removeChild(children_.begin());
}

void ceph::Actor::detach()
{
	if (hasParent())
		parent_.lock()->removeChild(shared_from_this());
	if (isInScene())
		scene_->removeActor(shared_from_this());
}

void ceph::Actor::detachFromScene()
{
	scene_ = nullptr;
	for (auto child : children_)
		child->detachFromScene();
}

void ceph::Actor::attachToScene( ceph::Scene& scene)
{
	scene_ = &scene;
	for (auto child : children_)
		child->attachToScene(scene);
}

bool ceph::Actor::isInScene() const
{
	return scene_;
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

void ceph::Actor::setActorState(const ActorState & state)
{
	state_ = state;
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
	moveTo(
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

ceph::Scene& ceph::Actor::getScene() const
{
	return *scene_;
}

float ceph::Actor::getAlpha() const
{
	return state_.getAlpha();
}

void ceph::Actor::setAlphaTo(float alpha)
{
	auto diff = alpha - state_.getAlpha();
	changeAlphaBy(diff);
}

void ceph::Actor::changeAlphaBy(float alpha_diff)
{
	state_.changeAlphaBy(alpha_diff);
	if (actions_.isRunning())
		actions_.changeAlphaBy(alpha_diff);
}

float ceph::Actor::getRotation() const
{
	return state_.getRotation();
}

void ceph::Actor::rotateTo(float radians)
{
	auto angle_delta = ceph::normalizeAngle(radians) - state_.getRotation();
	rotateBy(angle_delta);
}

void ceph::Actor::rotateBy(float angle_delta)
{
	state_.rotateBy(angle_delta);
	if (actions_.isRunning())
		actions_.rotateBy(angle_delta);
}

ceph::Vec2<float> ceph::Actor::getScale() const
{
	return state_.getScale();
}

void ceph::Actor::setScaleTo(float scale)
{
	setScaleTo(ceph::Vec2<float>(scale, scale));
}

void ceph::Actor::setScaleTo(const ceph::Vec2<float>& s)
{
	auto scale_delta = s - state_.getScale();
	changeScaleBy(scale_delta);
}

void ceph::Actor::changeScaleBy(float scale)
{
	changeScaleBy(Vec2<float>(scale, scale));
}

void ceph::Actor::changeScaleBy(const ceph::Vec2<float>& s)
{
	state_.changeScaleBy(s);
	if (actions_.isRunning())
		actions_.changeScaleBy(s);
}

ceph::Vec2<float> ceph::Actor::getPosition() const
{
	return state_.getPosition();
}

void ceph::Actor::moveTo(const ceph::Vec2<float>& position)
{
	auto delta = position - state_.getPosition();
	moveBy(delta);
}

void ceph::Actor::moveTo(float x, float y)
{
	moveTo(ceph::Vec2<float>(x, y));
}

void ceph::Actor::moveBy(const ceph::Vec2<float>& delta)
{
	state_.moveBy(delta);
	if (actions_.isRunning())
		actions_.moveBy(delta);
}

void ceph::Actor::moveBy(float x, float y)
{
	moveBy(Vec2<float>(x, y));
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

void ceph::Actor::applyConstraint(const std::shared_ptr<ceph::ActionConstraint>& constraint)
{
	constraints_.push_back(constraint);
}

void ceph::Actor::clearConstraints()
{
	constraints_.clear();
}

void ceph::Actor::enforceConstraints()
{
	if (!constraints_.empty())
		for (auto constraint : constraints_)
			constraint->apply(*this);
	else
		for (auto child : children_)
			child->enforceConstraints();
}

ceph::Actor::~Actor()
{
}
