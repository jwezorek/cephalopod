#include "../include/cephalopod/actionplayer.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actions.hpp"
#include "util.hpp"

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
			ai.complete = true;
		}
	}
	setActorState(state);
	if (has_completed_actions)
	{
		auto completed_begin = std::remove_if(
			actions_.begin(), actions_.end(),
			[](ActionInProgress& aip) {
				return aip.complete;
			}
		);

		for (auto i = completed_begin; i != actions_.end(); i++) {
			auto& completed_action_info = *i;
			auto& completed_action = i->action;

			// make the action-complete signal go off...
			completed_action_info.signal.fire( *completed_action );

			//burn the final state of the action into the cached state of the sprite...
			completed_action->update( *initial_actor_state_, 1.0f);
		}

		// actually delete the completed items in the vector
		actions_.erase(completed_begin, actions_.end());

		if (actions_.empty())
		{
			auto scene = parent_.getScene().lock();
			scene->updateEvent.disconnect(*this);
		}
	}
}

void  ceph::ActionPlayer::setActorState(const ActorState& state)
{
	parent_.setPosition(state.getTranslation());
	parent_.setAlpha(state.getAlpha());
	parent_.setRotation(state.getRotation());
	parent_.setScale(state.getScale());
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

void  ceph::ActionPlayer::applyAction(const std::shared_ptr<ceph::Action>& action)
{
	bool wasRunning = isRunning();
	actions_.push_back(action);
	if (!wasRunning && parent_.isInScene()) 
		run();
}

void  ceph::ActionPlayer::applyActions(std::initializer_list<std::shared_ptr<Action>> actions)
{
	for (auto& action : actions)
		applyAction(action);
}

void  ceph::ActionPlayer::removeAction(const std::shared_ptr<ceph::Action>& removee)
{

}