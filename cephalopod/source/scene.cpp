#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/drawingcontext.hpp"
#include "graphics.hpp"
#include "util.hpp"

namespace
{
	ceph::Rect<float> GetBkgdRect(ceph::BackgroundMode mode, float tex_wd, float tex_hgt)
	{
		auto log_rect = ceph::Game::getInstance().getLogicalRect();
		ceph::Rect<float> r;
		switch (mode) {
			case ceph::BackgroundMode::StretchToFit:
				r = log_rect;
				break;
			case ceph::BackgroundMode::PreserveWidth:
				break;

			case ceph::BackgroundMode::PreserveHeight:
				break;
			case ceph::BackgroundMode::Tile:
				r = ceph::Rect<float>(log_rect.x, log_rect.y, tex_wd, tex_hgt);
				break;
		}
		return r;
	}
}

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

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex, ceph::BackgroundMode bk_mode)
{
	bkgd_ = tex;
	bkgd_mode_ = bk_mode;
	bkgd_rect_ = GetBkgdRect(bk_mode, tex->getWidth(), tex->getHeight());
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
		dc.graphics.Blit(bkgd_rect_, bkgd_->getBounds(), 1.0f);
	}
}

ceph::Scene::~Scene()
{
}
