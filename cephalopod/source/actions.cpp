#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actor.hpp"
#include "util.hpp"

ceph::Action::Action(bool startPaused)
{
	is_paused_ = startPaused;
	owner_ = std::weak_ptr<Actor>();
}

void ceph::Action::attach(const std::shared_ptr<Actor> owner)
{
	owner_ = owner;
	for (auto& child : children_)
		child->attach(owner);
}

void ceph::Action::unattach()
{
	owner_ = std::weak_ptr<Actor>();
	for (auto& child : children_)
		child->unattach();
}

bool ceph::Action::isAttached() const
{
	return !owner_.expired();
}

void ceph::Action::pause()
{
	is_paused_ = true;
	for (auto& child : children_)
		child->pause();
}

void ceph::Action::unpause()
{
	is_paused_ = false;
	for (auto& child : children_)
		child->unpause();
}

bool ceph::Action::isPaused() const
{
	return is_paused_;
}

bool ceph::Action::hasChildren() const
{
	return !children_.empty();
}

void ceph::Action::run(const std::shared_ptr<Actor>& actor)
{
	owner_ = actor;
	for (auto& child : children_)
		child->run(actor);
}

ceph::Action::~Action()
{
}

/*--------------------------------------------------------------------------------*/

ceph::FiniteAction::FiniteAction(float duration, bool startPaused) : Action(startPaused)
{
	duration_ = duration;
	elapsed_ = 0.0f;
	is_complete_ = false;
}

void ceph::FiniteAction::run(const std::shared_ptr<Actor>& actor)
{
	Action::run(actor);
	elapsed_ = 0.0f;
	is_complete_ = false;
}

void ceph::FiniteAction::update(float elapsed)
{
	if (isComplete() || isPaused())
		return;
	elapsed_ += elapsed;
	float pcnt_complete = elapsed_ / duration_;
	if (pcnt_complete >= 1.0f) 
		pcnt_complete = 1.0f;

	setActionState(pcnt_complete);
}

void ceph::FiniteAction::setActionState(float pcnt_complete)
{
	setSpriteState(pcnt_complete);
	if (pcnt_complete == 1.0f) {
		is_complete_ = true;
		complete_event_.fire(this);
	}
}

bool ceph::FiniteAction::isComplete() const
{
	return is_complete_;
}

/*--------------------------------------------------------------------------------*/

ceph::MoveToAction::MoveToAction(float duration, const ceph::Point<float>& dest, bool startPaused) :
	FiniteAction(duration, startPaused), start_(ceph::Point<float>(0,0)), dest_(dest)
{
}

void ceph::MoveToAction::run(const std::shared_ptr<Actor>& actor)
{
	FiniteAction::run(actor);
	start_ = owner_.lock()->getPosition();
}

void ceph::MoveToAction::setSpriteState(float pcnt_complete)
{
	auto pt = ceph::Point<float>(
		ceph::lerp(start_.x, dest_.x, pcnt_complete),
		ceph::lerp(start_.y, dest_.y, pcnt_complete)
	);
	owner_.lock()->setPosition(pt);
}