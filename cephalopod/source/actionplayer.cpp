#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/actorstate.hpp"
#include "../include/cephalopod/actionplayer.hpp"
#include "util.hpp"

ceph::ActionPlayer::ActionInProgress::ActionInProgress(int id, const ceph::Action & a, bool rep) :
	id(id),
	action(a),
	elapsed(0.0f),
	complete(false),
	repeat(rep)
{
}

float ceph::ActionPlayer::ActionInProgress::getPcntComplete() const
{
	return elapsed / action.getDuration();
}

void ceph::ActionPlayer::finalizeAction(ceph::ActionPlayer::ActionInProgress& completed_action_info, bool emit_signal)
{
	auto& action = completed_action_info.action;
	//burn the final state of the action into the cached state of the sprite...
	action(*initial_actor_state_, completed_action_info.getPcntComplete());

	completed_action_info.complete = true;
	completed_action_info.elapsed = 0.0f;

	// make the action-complete signal go off...
	if (emit_signal)
		completed_action_info.signal.fire(completed_action_info.id);
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
		initial_actor_state_ = std::make_unique<ceph::ActorState>(parent_.getState());
}

void ceph::ActionPlayer::removeActions(const std::function<bool(const ActionInProgress&)> predicate)
{
	actions_.erase(
		std::remove_if(
			actions_.begin(),
			actions_.end(),
			predicate
		), 
		actions_.end()
	);
	//actions now contains nothing or a mixture of incomplete actions and complete actions that are repeat-enabled.
	if (actions_.empty()) {
		// if there are no more actions then unhook the action player object from the scene's update event
		initial_actor_state_ = nullptr;
		Game::getInstance().getActiveScene()->updateActionsEvent.disconnect(*this);
	} else {
		// toggle the complete flag of repeating items and if there are only repeating items
		// left, re-set the cached state...
		resetActions();
	}
}

void ceph::ActionPlayer::update(float dt)
{
	bool has_completed_actions = false;
	ceph::ActorState state( *initial_actor_state_ );
	for (auto& ai: actions_) {
		float duration = ai.action.getDuration();
		ai.elapsed = (ai.elapsed + dt < duration) ? ai.elapsed + dt : duration;
		ai.action( state, ai.getPcntComplete() );

		if (ai.elapsed == duration) {
			has_completed_actions = true;
			finalizeAction(ai);
		}
	}

	setActorState(state);

	if (has_completed_actions) {
		// remove the completed non-repeating actions and then clean up...
		removeActions(
			[](const ActionInProgress& aip) {
				return aip.complete && !aip.repeat;
			}
		);
	}
}

void ceph::ActionPlayer::setActorState(const ActorState& state)
{
	parent_.setPosition( state.getPosition() );
	parent_.setRotation( state.getRotation() );
	parent_.setScale( state.getScale() );
	parent_.setAlpha(state.getAlpha());

	auto sprite_frame = state.getSpriteFrame();
	if (!sprite_frame.empty()) {
		static_cast<ceph::Sprite*>(&parent_)->setFrame(sprite_frame);
	}
}

ceph::ActionPlayer::ActionPlayer(Actor& parent) :
	parent_(parent)
{
}

void ceph::ActionPlayer::run()
{
	auto scene = parent_.getScene().lock();
	scene->updateActionsEvent.connect(*this, &ceph::ActionPlayer::update);
	initial_actor_state_ = std::make_unique<ceph::ActorState>(parent_.getState());
}

bool ceph::ActionPlayer::isRunning() const
{
	return hasActions() && parent_.isInScene();
}

bool ceph::ActionPlayer::hasActions() const
{
	return !actions_.empty();
}

void ceph::ActionPlayer::applyAction(int id, const ceph::Action& action, bool repeat)
{
	bool wasRunning = isRunning();
	actions_.push_back({id, action, repeat });
	if (!wasRunning && parent_.isInScene())
		run();
}

void ceph::ActionPlayer::applyAction(const ceph::Action& action, bool repeat)
{
	applyAction(-1, action, repeat);
}

void ceph::ActionPlayer::applyActions(std::initializer_list<ceph::Action> actions)
{
	for (auto& action : actions)
		applyAction(action);
}

void ceph::ActionPlayer::removeAction(int id)
{
	bool found_one = false;
	for (auto& a : actions_) {
		if (a.id == id) {
			finalizeAction(a, false);
			found_one = true;
		}
	}
	if (found_one) {
		removeActions(
			[id](const ActionInProgress& ai) {return ai.id == id; }
		);
	}
}

ceph::Signal<int>& ceph::ActionPlayer::getCompletionSignal(int id)
{
	auto ia = std::find_if(
		actions_.begin(), actions_.end(),
				[id](const auto& aip) {
				return aip.id == id;
		}
	);
	return ia->signal;
}

bool ceph::ActionPlayer::hasAction(int id)
{
	return (
		std::find_if(
			actions_.cbegin(), actions_.cend(),
			[id](const auto& aip) {
				return aip.id == id;
			}
		) != actions_.cend()
	);
}

void  ceph::ActionPlayer::clearActions()
{
	for(auto& ai : actions_)
		finalizeAction(ai, false);
	removeActions(
		[](const ActionInProgress& a)->bool {return true; }
	);
}



