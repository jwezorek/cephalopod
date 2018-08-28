#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/drawingcontext.hpp"
#include "graphics.hpp"
#include "util.hpp"

ceph::Scene::Scene()
{
}

void ceph::Scene::endGameLoopIteration()
{
	dropped_actors_.clear();
}

void ceph::Scene::update(float dt)
{
	updateActionsEvent.fire( dt );
	updateEvent.fire( dt );
	for (auto actor : stage_)
		actor->enforceConstraints();
}

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex)
{
	

}

ceph::Mat3x3 CreateStretchToFitMatrix(float logical_wd, float logical_hgt)
{
	return ceph::Mat3x3().scale(
		2.0f / logical_wd,
		2.0f / logical_hgt
	);
}

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex, ceph::CoordinateMapping mapping)
{
	bkgd_ = tex;
	bkgd_tranform_ = CreateStretchToFitMatrix(tex->getWidth(), tex->getHeight());
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
	
	if (child->isInSceneTopLevel() && child->hasActions())
		child->runActions();
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

void ceph::Scene::draw(ceph::DrawingContext& dc)
{
	//TODO: paint bkgd color...
	drawBackground(dc);
	for (const auto& actor : stage_)
		actor->draw(dc);
}

void ceph::Scene::drawBackground(ceph::DrawingContext& dc)
{
	if (bkgd_.get()) {
		auto curr_tex = dc.graphics.GetCurrentTexture();
		if (curr_tex.get() != bkgd_.get())
			dc.graphics.SetCurrentTexture(bkgd_);
		dc.graphics.Blit(bkgd_tranform_, curr_tex->getBounds(), 1.0f);
	}
}

ceph::Scene::~Scene()
{
}
