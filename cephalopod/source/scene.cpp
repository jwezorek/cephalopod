#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/drawingcontext.hpp"
#include "graphics.hpp"
#include "util.hpp"
#include <cmath>

namespace
{
	ceph::Vec2<float> GetCenterOfRect(const ceph::Rect<float>& r)
	{
		return ceph::Vec2<float>(
			(r.x + r.x2()) / 2.0f,
			(r.y + r.y2()) / 2.0f
		);
	}

	ceph::Rect<float> CenterRectAroundPoint(const ceph::Rect<float>& r, const ceph::Vec2<float>& pt)
	{
		return ceph::Rect<float>(
			pt.x - r.wd / 2.0f,
			pt.y - r.hgt / 2.0f,
			r.wd,
			r.hgt
		);
	}

	ceph::Rect<float> GetBkgdRect(ceph::BackgroundMode mode, float tex_wd, float tex_hgt)
	{
		auto log_rect = ceph::Game::getInstance().getLogicalRect();

		ceph::Rect<float> r;
		switch (mode) {
			case ceph::BackgroundMode::StretchToFit:
				r = log_rect;
				break;

			case ceph::BackgroundMode::PreserveWidth: {
					float aspect_ratio = tex_hgt / tex_wd;
					float new_hgt = log_rect.wd * aspect_ratio;
					r = CenterRectAroundPoint(
						ceph::Rect<float>(0,0,log_rect.wd,new_hgt),
						GetCenterOfRect(log_rect)
					);
				}	
				break;

			case ceph::BackgroundMode::PreserveHeight: {
					float inv_aspect_ratio = tex_wd / tex_hgt;
					float new_wd = log_rect.hgt * inv_aspect_ratio;
					r = CenterRectAroundPoint(
						ceph::Rect<float>(0, 0, new_wd, log_rect.hgt),
						GetCenterOfRect(log_rect)
					);
				}
				break;

			case ceph::BackgroundMode::Tile:
				r = ceph::Rect<float>(log_rect.x, log_rect.y, tex_wd, tex_hgt);
				break;
		}
		return r;
	}

	void DrawTiledBackground(ceph::Graphics& g, ceph::Vec2<int> tile_sz)
	{
		ceph::Rect<int> tile_rect(0, 0, tile_sz.x, tile_sz.y);
		auto log_rect = ceph::Game::getInstance().getLogicalRect();
		int columns = static_cast<int>(std::ceil(log_rect.wd / tile_sz.x));
		int rows = static_cast<int>(std::ceil(log_rect.hgt / tile_sz.y));
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < columns; col++) {
				ceph::Rect<float> r(
					log_rect.x + col * tile_sz.x, log_rect.y + row * tile_sz.y,
					tile_sz.x, tile_sz.y
				);
				g.Blit(r, tile_rect, 1.0f);
			}
		}
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
	dc.graphics.Clear(bkgd_color_, true);
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

		if (bkgd_mode_ != ceph::BackgroundMode::Tile)
			dc.graphics.Blit(bkgd_rect_, bkgd_->getBounds(), 1.0f);
		else
			DrawTiledBackground(dc.graphics, bkgd_->getSize()); //TODO: possibly do this via texture wrapMode
	}
}

ceph::Scene::~Scene()
{
}
