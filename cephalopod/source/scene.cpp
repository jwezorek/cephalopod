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

void ceph::Scene::handleKeyEvent(bool isPressed, KeyCode key, unsigned char modifiers)
{
	if (isPressed && key == KeyCode::Escape)
		ceph::Game::getInstance().quit();
}

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex)
{
	setBackground(tex, ceph::Game::getInstance().getCoordinateMapping());
}

void ceph::Scene::setBackground(const std::shared_ptr<Texture>& tex, ceph::CoordinateMapping mapping)
{
	impl_->setBackground(tex, mapping);
}

void ceph::Scene::scheduleUpdate(Actor& actor, const UpdateHandlerFunc & func)
{
	impl_->updateHandlers_.push_back(
		std::pair<Actor*, UpdateHandlerFunc>(&actor, func)
	);
}

void ceph::Scene::unscheduleUpdate(Actor&)
{
}

void ceph::Scene::registerKeyEventHandler(Actor&, const KeyEventHandlerFunc & func)
{
}

void ceph::Scene::unregisterKeyEvenHandler(Actor&)
{
}

void ceph::Scene::addActor(const std::shared_ptr<ceph::Actor>& child)
{
	impl_->stage_.push_back(child);
}

void ceph::Scene::update(float elapsed)
{
	for (auto& item : impl_->updateHandlers_)
		item.second(*(item.first), elapsed);
}

void ceph::Scene::draw(DrawingContext && rt)
{
	impl_->drawBackground(rt);
	for (const auto& actor : impl_->stage_)
		actor->draw(rt);
}

ceph::Scene::~Scene()
{
}
