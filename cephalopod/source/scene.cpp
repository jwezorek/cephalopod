#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/texture.hpp"
#include "textureimpl.hpp"
#include "sceneimpl.hpp"
#include "util.hpp"

ceph::Scene::Scene()
{
	impl_ = std::make_unique<SceneImpl>();
}

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex)
{
	setBackground(tex, ceph::Game::getInstance().getCoordinateMapping());
}

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex, ceph::CoordinateMapping mapping)
{
	impl_->setBackground(tex, mapping);
}

void ceph::Scene::addActor(const std::shared_ptr<ceph::Actor>& child)
{
	stage_.push_back(child);
	child->attachToScene( shared_from_this() );
	if (child->hasActions())
		child->runActions();
}

void ceph::Scene::removeActor(const std::shared_ptr<ceph::Actor>& child)
{
	auto i = std::find(stage_.begin(), stage_.end(), child);
	if (i == stage_.end())
		return;

	(*i)->detachFromScene();
	stage_.erase(i);
}

void ceph::Scene::draw(DrawingContext && rt)
{
	impl_->drawBackground(rt);
	for (const auto& actor : stage_)
		actor->draw(rt);
}

ceph::Scene::~Scene()
{
}
