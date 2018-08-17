#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/drawingcontext.hpp"
#include "util.hpp"

ceph::Scene::Scene()
{
}

void ceph::Scene::endGameLoopIteration()
{
	dropped_actors_.clear();
}

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex)
{
	//setBackground(tex, ceph::Game::getInstance().getCoordinateMapping());
}

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex, ceph::CoordinateMapping mapping)
{
	//impl_->setBackground(tex, mapping);
}

void ceph::Scene::setBackgroundColor(const ceph::ColorRGB& color)
{
	bkgd_color_ = color;
}

ceph::ColorRGB ceph::Scene::getBackgroundColor() const
{
	return bkgd_color_;
}

void ceph::Scene::addActor(const std::shared_ptr<ceph::Actor>& child, bool add_on_top)
{
	if (add_on_top)
		stage_.push_back(child);
	else
		stage_.push_front(child);

	child->attachToScene( shared_from_this() );
	/*
	if (child->isInSceneTopLevel() && child->hasActions())
		child->runActions();
	*/
}

void ceph::Scene::addActors(std::initializer_list<std::shared_ptr<Actor>> children, bool add_on_top)
{
	for (const auto& child : children)
		addActor(child);
}

void ceph::Scene::removeActor(const std::shared_ptr<ceph::Actor>& child)
{
	auto i = std::find(stage_.begin(), stage_.end(), child);
	if (i == stage_.end())
		return;

	(*i)->detachFromScene();
	stage_.erase(i);
}

void ceph::Scene::draw(ceph::DrawingContext& rt)
{
	//drawBackground(rt);
	for (const auto& actor : stage_)
		actor->draw(rt);
}

ceph::Scene::~Scene()
{
}
