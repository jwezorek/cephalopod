#include "actionplayer.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actions.hpp"
#include "util.hpp"
#include "actorstate.hpp"

void ceph::ActionPlayer::markActionAsComplete(ceph::ActionPlayer::ActionInProgress& completed_action_info)
{
	auto& action = completed_action_info.action;
	completed_action_info.complete = true;
	completed_action_info.elapsed = 0.0f;

	// make the action-complete signal go off...
	completed_action_info.signal.fire( *action );

	//burn the final state of the action into the cached state of the sprite...
	action->update(*initial_actor_state_, 1.0f);
}

void ceph::ActionPlayer::resetActions()
{
	bool has_only_complete_actions = true;
	for (auto& a : actions_) {
		if (a.complete)
			a.complete = false;
		else
			has_only_complete_actions = false;
	}
	if (has_only_complete_actions)
		initial_actor_state_ = std::make_unique<ceph::ActorState>(parent_);
}

void ceph::ActionPlayer::update(float dt)
{
	bool has_completed_actions = false;
	ceph::ActorState state( *initial_actor_state_ );
	for (auto& ai: actions_) {
		float duration = ai.action->getDuration();
		ai.elapsed = (ai.elapsed + dt < duration) ? ai.elapsed + dt : duration;
		ai.action->update( state, ai.elapsed/duration );

		if (ai.elapsed == duration) {
			has_completed_actions = true;
			markActionAsComplete(ai);
		}
	}

	setActorState(state);
	applyConstraints(parent_);

	if (has_completed_actions)
	{
		// actually delete the completed, non-repeating items...
		actions_.erase(std::remove_if(
				actions_.begin(), 
				actions_.end(),
				[](ActionInProgress& aip) {
					return aip.complete && !aip.repeat;
				}
			), actions_.end()
		);
		//actions now contains nothing or a mixture of incomplete actions and complete actions that are repeat-enabled.
		if (actions_.empty()) {
			// if there are no more actions then unhook the action player object from the scene's update event
			initial_actor_state_ = nullptr;
			auto scene = parent_.getScene().lock();
			scene->updateEvent.disconnect(*this);
		} else {
			// toggle the complete flag of repeating items and if there are only repeating items
			// left, re-set the cached state...
			resetActions();
		}
	}
}

void ceph::ActionPlayer::setActorState(const ActorState& state)
{
	parent_.setPosition(state.getTranslation());
	parent_.setAlpha(state.getAlpha());
	parent_.setRotation(state.getRotation());
	parent_.setScale(state.getScale());
}


void ceph::ActionPlayer::applyConstraints(Actor& actor)
{
	for (const auto& constraint : constraints_)
		constraint->apply(actor);
}

ceph::ActionPlayer::ActionPlayer(Actor& parent) : 
	parent_(parent)
{
}

void ceph::ActionPlayer::run()
{
	auto scene = parent_.getScene().lock();
	scene->updateEvent.connect(*this, &ceph::ActionPlayer::update);
	initial_actor_state_ = std::make_unique<ceph::ActorState>(parent_);
}

bool ceph::ActionPlayer::isRunning() const
{
	return hasActions() && parent_.isInScene();
}

bool ceph::ActionPlayer::hasActions() const
{
	return !actions_.empty();
}

void ceph::ActionPlayer::applyAction(const std::shared_ptr<ceph::Action>& action, bool repeat)
{
	bool wasRunning = isRunning();
	actions_.push_back({action, repeat});
	if (!wasRunning && parent_.isInScene()) 
		run();
}

void ceph::ActionPlayer::applyActions(std::initializer_list<std::shared_ptr<Action>> actions)
{
	for (auto& action : actions)
		applyAction(action);
}

void ceph::ActionPlayer::applyConstraint(const std::shared_ptr<ceph::ActionConstraint>& constraint)
{
	constraints_.push_back(constraint);
}

void  ceph::ActionPlayer::removeAction(const std::shared_ptr<ceph::Action>& removee)
{

}