#define _USE_MATH_DEFINES
#include <cmath>
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
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

ceph::RotateAction::RotateAction(float ang_vel) :ang_velocity_(ang_vel)
{
}

void ceph::RotateAction::update(float timestep)
{
	auto sprite = owner_.lock();
	float rot = sprite->getRotation();
	rot += ang_velocity_ * timestep;
	//TODO: do the following correctly...
	if (rot < 0.0f)
		rot += 2.0f*M_PI;
	if (rot > 2.0f*M_PI)
		rot -= 2.0f*M_PI;
	sprite->setRotation(rot);
}

/*--------------------------------------------------------------------------------*/

ceph::MoveAction::MoveAction(float x, float y) : velocity_(ceph::Vec2D<float>(x, y)) {}
ceph::MoveAction::MoveAction(const Vec2D<float>& vel) : velocity_(vel) {}
void ceph::MoveAction::update(float timestep)
{
	auto sprite = owner_.lock();
	auto p = sprite->getPosition();
	sprite->setPosition(p.x + timestep * velocity_.x, p.y + timestep * velocity_.y);
}

/*--------------------------------------------------------------------------------*/

ceph::MoveWithWrappingAction::MoveWithWrappingAction(float x, float y) : ceph::MoveAction(x,y) {}
ceph::MoveWithWrappingAction::MoveWithWrappingAction(const ceph::Vec2D<float>& vel) : ceph::MoveAction(vel) {}
void ceph::MoveWithWrappingAction::update(float timestep)
{
	MoveAction::update(timestep);

	auto& game = ceph::Game::getInstance();
	auto sprite = owner_.lock();
	auto screen_rect = game.getScreenRect();
	auto scr_coord_bounds = game.convertToScreenCoords(sprite->getGlobalBounds());

	if (!screen_rect.intersects(scr_coord_bounds)) {
		auto new_bounds = scr_coord_bounds;
		if (new_bounds.x2() < 0.0f && velocity_.x < 0)
			new_bounds.x = screen_rect.wd;
		else if (new_bounds.x > screen_rect.wd && velocity_.x > 0)
			new_bounds.x = -scr_coord_bounds.wd;
		if (new_bounds.y2() < 0.0f && velocity_.y < 0)
			new_bounds.y = screen_rect.hgt;
		else if (new_bounds.y > screen_rect.hgt && velocity_.y > 0)
			new_bounds.y = -scr_coord_bounds.hgt;
		auto new_position = game.convertFromScreenCoords(ceph::lerpPtInRect(sprite->getAnchorPt(), new_bounds));

		sprite->setGlobalPosition(new_position);
	}
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

	float new_elapsed = (elapsed_ + timestep <= duration_) ? elapsed_ + timestep : duration_;
	doTimeStep(new_elapsed - elapsed_);
	if (new_elapsed == duration_)
		setComplete();

	elapsed_ = new_elapsed;
}

float ceph::FiniteAction::getDuration() const
{
	return duration_;
}

float ceph::FiniteAction::getElapsed() const
{
	return elapsed_;
}


bool ceph::FiniteAction::isComplete() const
{
	return is_complete_;
}

void ceph::FiniteAction::setComplete()
{
	if (is_complete_)
		return;
	for (auto child : children_)
		std::static_pointer_cast<FiniteAction>(child)->setComplete();
	is_complete_ = true;
	complete_event_.fire(*this);
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

void ceph::MoveByAction::doTimeStep(float timestep)
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

void ceph::SetTransparencyToAction::doTimeStep(float timestep)
{
	auto actor = owner_.lock();
	actor->setAlpha(actor->getAlpha() + alpha_change_rate_ * timestep);
}

/*--------------------------------------------------------------------------------*/

ceph::SequenceAction::SequenceAction(std::initializer_list<std::shared_ptr<ceph::FiniteAction>> actions, bool startPaused) :
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

void ceph::SequenceAction::doTimeStep(float timestep) {
	float new_elapsed = elapsed_ + timestep;

	auto start_action_iter = getCurrentAction(elapsed_);
	auto end_action_iter = getCurrentAction(new_elapsed);

	std::shared_ptr<FiniteAction> current_action;
	if (start_action_iter == end_action_iter) {
		current_action = std::static_pointer_cast<FiniteAction>(start_action_iter->second);
		float action_start_time = start_action_iter->first;
		current_action->doTimeStep( timestep);
	} else {
		auto old_action = start_action_iter->second;
		float old_action_start_time = start_action_iter->first;

		current_action = (end_action_iter != start_tbl_.end()) ? end_action_iter->second : nullptr;
		float new_action_start_time = (end_action_iter != start_tbl_.end()) ? end_action_iter->first : duration_;

		old_action->doTimeStep(new_action_start_time - elapsed_);
		old_action->setComplete();

		if (current_action)
			current_action->doTimeStep(timestep - (new_action_start_time - elapsed_));
	}
	if (current_action && current_action->getElapsed() >= current_action->getDuration())
		current_action->setComplete();
}