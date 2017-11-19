#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/scene.hpp"
#include "SFML/Graphics.hpp"
#include "drawingcontext.hpp"
#include "actorimpl.hpp"
#include "util.hpp"

ceph::Actor::Actor() :
	impl_(std::make_unique<ActorImpl>())
{
}

void ceph::Actor::AddChild(const std::shared_ptr<ceph::Actor>& actor)
{
	children_.push_back(actor);
	if (isInScene())
		actor->attachToScene(scene_.lock());
}

void ceph::Actor::RemoveChild(const std::shared_ptr<ceph::Actor>& actor)
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
		parent_.lock()->RemoveChild(shared_from_this());
	else if (isInScene())
		scene_.lock()->removeActor(shared_from_this());
}

void ceph::Actor::detachFromScene()
{
	scene_ = std::weak_ptr<Scene>();
	for (auto child : children_)
		child->detachFromScene();
}

void ceph::Actor::attachToScene(const std::shared_ptr<Scene>& scene)
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

float ceph::Actor::getAlpha() const
{
	return 0.0f;
}

void ceph::Actor::setAlpha(float alpha)
{
	impl_->alpha = alpha;
}

float ceph::Actor::getRotation() const
{
	return ceph::DegreesToRadians(impl_->properties.getRotation());
}

void ceph::Actor::setRotation(float radians)
{
	impl_->properties.setRotation(ceph::RadiansToDegrees(radians));
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

ceph::Point<float> ceph::Actor::getPosition() const
{
	auto origin = impl_->properties.getPosition();
	return ceph::Point<float>(origin.x, origin.y);
}

void ceph::Actor::setPosition(const ceph::Point<float>& pt)
{
	impl_->properties.setPosition(sf::Vector2f(pt.x, pt.y));
}

void ceph::Actor::setPosition(float x, float y)
{
	impl_->properties.setPosition(sf::Vector2f(x, y));
}

ceph::Point<float> ceph::Actor::getAnchorPt() const
{
	auto origin = impl_->properties.getOrigin();
	return ceph::Point<float>(origin.x, origin.y);
}

void ceph::Actor::setAnchorPt(const ceph::Point<float>& pt)
{
	setAnchorPt(pt.x, pt.y);
}

void ceph::Actor::setAnchorPt(float x, float y)
{
	impl_->anchor = ceph::Point<float>(x,y);
	impl_->properties.setOrigin(
		x * static_cast<float>(impl_->frame_sz.wd),
		y * static_cast<float>(impl_->frame_sz.hgt)
	);
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
