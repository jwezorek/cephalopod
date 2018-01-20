#include "actionplayerimpl.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/game.hpp"
#include "util.hpp"
#include "actorstate.hpp"
#include "actorimpl.hpp"

ceph::ActionPlayerImpl::ActionInProgress::ActionInProgress(int id, const ceph::Action & a, bool rep) :
	id(id),
	action(a),
	elapsed(0.0f),
	complete(false),
	repeat(rep)
{
}

float ceph::ActionPlayerImpl::ActionInProgress::getPcntComplete() const
{
	return elapsed / action.getDuration();
}

void ceph::ActionPlayerImpl::finalizeAction(ceph::ActionPlayerImpl::ActionInProgress& completed_action_info, bool emit_signal)
{
	auto& action = completed_action_info.action;
	//burn the final state of the action into the cached state of the sprite...
	action(*initial_actor_state_, completed_action_info.getPcntComplete());
	applyConstraints(*initial_actor_state_);

	completed_action_info.complete = true;
	completed_action_info.elapsed = 0.0f;

	// make the action-complete signal go off...
	if (emit_signal)
		completed_action_info.signal.fire(completed_action_info.id);
}

void ceph::ActionPlayerImpl::resetActions()
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

void ceph::ActionPlayerImpl::removeActions(const std::function<bool(const ActionInProgress&)> predicate)
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

void ceph::ActionPlayerImpl::update(float dt)
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

	applyConstraints(state);
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

void ceph::ActionPlayerImpl::setActorState(const ActorState& state)
{
	auto& transformable = parent_.impl_->properties;
	transformable.setPosition( state.getPosition() );
	transformable.setRotation( state.getRotation() );
	transformable.setScale( state.getScale() );
}


void ceph::ActionPlayerImpl::applyConstraints(ActorState& state)
{
	for (const auto& constraint : constraints_)
		constraint->apply(state);
}

ceph::ActionPlayerImpl::ActionPlayerImpl(Actor& parent) :
	parent_(parent)
{
}

void ceph::ActionPlayerImpl::run()
{
	auto scene = parent_.getScene().lock();
	scene->updateActionsEvent.connect(*this, &ceph::ActionPlayerImpl::update);
	initial_actor_state_ = std::make_unique<ceph::ActorState>(parent_);
}

bool ceph::ActionPlayerImpl::isRunning() const
{
	return hasActions() && parent_.isInScene();
}

bool ceph::ActionPlayerImpl::hasActions() const
{
	return !actions_.empty();
}

void ceph::ActionPlayerImpl::applyAction(int id, const ceph::Action& action, bool repeat)
{
	bool wasRunning = isRunning();
	actions_.push_back({id, action, repeat });
	if (!wasRunning && parent_.isInScene())
		run();
}

void ceph::ActionPlayerImpl::applyAction(const ceph::Action& action, bool repeat)
{
	applyAction(-1, action, repeat);
}

void ceph::ActionPlayerImpl::applyActions(std::initializer_list<Action> actions)
{
	for (auto& action : actions)
		applyAction(action);
}

void ceph::ActionPlayerImpl::applyConstraint(const std::shared_ptr<ceph::ActionConstraint>& constraint)
{
	constraints_.push_back(constraint);
}

void ceph::ActionPlayerImpl::removeAction(int id)
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

ceph::Signal<int>& ceph::ActionPlayerImpl::getCompletionSignal(int id)
{
	auto ia = std::find_if(
		actions_.begin(), actions_.end(),
				[id](const auto& aip) {
				return aip.id == id;
		}
	);
	return ia->signal;
}

bool ceph::ActionPlayerImpl::hasAction(int id)
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

void  ceph::ActionPlayerImpl::clearActions()
{
	for(auto& ai : actions_)
		finalizeAction(ai, false);
	removeActions(
		[](const ActionInProgress& a)->bool {return true; }
	);
}


void ceph::ActionPlayerImpl::clearConstraints()
{
	constraints_.clear();
}

void ceph::ActionPlayerImpl::enforceConstraints()
{
	ceph::ActorState state( parent_ );
	applyConstraints(state);
	setActorState(state);
}

void ceph::ActionPlayerImpl::translateCacheState(const ceph::Vec2D<float> offset)
{
	initial_actor_state_->translate(offset);
}


