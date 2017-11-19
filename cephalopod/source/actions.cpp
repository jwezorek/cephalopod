#include "../include/cephalopod/actions.hpp"

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

ceph::Action::~Action()
{
}

/*--------------------------------------------------------------------------------*/

ceph::FiniteAction::FiniteAction(float duration)
{
	duration_ = duration;
	elapsed_ = 0.0f;
	is_complete_ = false;
}

void ceph::FiniteAction::run() 
{
	elapsed_ = 0.0f;
	is_complete_ = false;
	for (auto& child : children_)
		child->run();
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