#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actor.hpp"
#include "util.hpp"
#include <map>
#include <numeric>

ceph::Action::Action(bool startPaused)
{
	is_paused_ = startPaused;
	owner_ = std::weak_ptr<Actor>();
}

bool ceph::Action::isRunning() const
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

void ceph::Action::stopRunning()
{
	owner_ = std::weak_ptr<Actor>();
	for (auto& child : children_)
		child->stopRunning();
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

void ceph::FiniteAction::update(float timestep)
{
	if (isComplete() || isPaused())
		return;

	elapsed_ = doTimestep(elapsed_, timestep);
}

float ceph::FiniteAction::doTimestep(float elapsed, float timestep)
{
	float new_elapsed = (elapsed + timestep <= duration_) ? elapsed + timestep : duration_;
	setSpriteState( elapsed, new_elapsed - elapsed);
	if (new_elapsed == duration_) {
		is_complete_ = true;
		complete_event_.fire(*this);
	}
	return new_elapsed;
}

float ceph::FiniteAction::getDuration() const
{
	return duration_;
}

bool ceph::FiniteAction::isComplete() const
{
	return is_complete_;
}

ceph::Signal<ceph::Action&>& ceph::FiniteAction::getCompletionEvent()
{
	return complete_event_;
}

/*--------------------------------------------------------------------------------*/

ceph::MoveByAction::MoveByAction(float duration, const ceph::Vec2D<float>& amount, bool startPaused) :
	FiniteAction(duration, startPaused)
{
	velocity_ = ceph::Vec2D<float>( amount.x / duration, amount.y / duration);
}

ceph::MoveByAction::MoveByAction(float duration, float x, float y, bool startPaused) :
	FiniteAction(duration, startPaused)
{
	velocity_ = ceph::Vec2D<float>(x / duration, y / duration);
}

void ceph::MoveByAction::setSpriteState(float elapsed, float timestep)
{
	auto sprite = owner_.lock();
	auto pt = sprite->getPosition();
	sprite->setPosition(
		pt.x + velocity_.x * timestep,
		pt.y + velocity_.y * timestep
	);
}

/*--------------------------------------------------------------------------------*/

ceph::MoveToAction::MoveToAction(float duration, const Point<float>& destination, bool startPaused) :
	MoveByAction(duration, ceph::Vec2D<float>(), startPaused), dest_(destination)
{
}

void ceph::MoveToAction::run(const std::shared_ptr<Actor>& actor)
{
	ceph::MoveByAction::run(actor);
	auto loc = actor->getPosition();
	velocity_ = ceph::Vec2D<float>(
		(dest_.x - loc.x) / duration_,
		(dest_.y - loc.y) / duration_
	);
}

/*--------------------------------------------------------------------------------*/

ceph::SetTransparencyToAction::SetTransparencyToAction(float duration, float alpha, bool startPaused) :
	FiniteAction(duration, startPaused)
{
	target_alpha_ = alpha;
}

void ceph::SetTransparencyToAction::run(const std::shared_ptr<Actor>& actor)
{
	ceph::FiniteAction::run(actor);
	alpha_change_rate_ = (target_alpha_ - actor->getAlpha()) / duration_;
}

void ceph::SetTransparencyToAction::setSpriteState(float elapsed, float timestep)
{
	auto actor = owner_.lock();
	actor->setAlpha(actor->getAlpha() + alpha_change_rate_ * timestep);
}

/*--------------------------------------------------------------------------------*/

ceph::SequenceAction::SequenceAction(const std::vector<std::shared_ptr<ceph::FiniteAction>>& actions, bool startPaused) :
	FiniteAction(0, actions, startPaused)
{
	float sum = 0.0f;
	for (auto action : actions) {
		start_tbl_[sum] = action;
		sum += action->getDuration();
	}
	duration_ = sum;
}

auto ceph::SequenceAction::getCurrentAction(float elapsed) const
{
	auto lb = start_tbl_.lower_bound(elapsed);
	return (lb != start_tbl_.end() && lb->first == elapsed) ? lb : std::prev(lb);
}

void ceph::SequenceAction::setSpriteState(float elapsed, float timestep) {
	float new_elapsed = (elapsed + timestep <= duration_) ? elapsed + timestep : duration_;

	auto start_action_iter = getCurrentAction(elapsed);
	auto end_action_iter = getCurrentAction(new_elapsed);

	if (start_action_iter == end_action_iter) {
		auto action = start_action_iter->second;
		float action_start_time = start_action_iter->first;
		action->doTimestep(elapsed - action_start_time, timestep);
	}
	else {
		auto old_action = start_action_iter->second;
		float old_action_start_time = start_action_iter->first;

		auto new_action = (end_action_iter != start_tbl_.end()) ? end_action_iter->second : nullptr;
		float new_action_start_time = (end_action_iter != start_tbl_.end()) ? end_action_iter->first : duration_;

		old_action->doTimestep(elapsed - old_action_start_time, new_action_start_time - elapsed);
		if (new_action)
			new_action->doTimestep(0.0f, timestep - (new_action_start_time - elapsed));
	}
}