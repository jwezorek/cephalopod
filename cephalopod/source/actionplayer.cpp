#include "../include/cephalopod/actionplayer.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/actions.hpp"

void ceph::ActionPlayer::update(float dt)
{
	ceph::ActorState state( *initial_actor_state_ );
	for (auto& ai: actions_) {
		float duration = ai.action->getDuration();
		float elapsed = ai.elapsed + dt;
		ai.elapsed = (elapsed < duration) ? elapsed : duration;
		ai.action->update(state, dt);
	}
	setActorState(state);
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

}

void  ceph::ActionPlayer::removeAction(const std::shared_ptr<ceph::Action>& removee)
{

}