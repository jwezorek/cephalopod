#include "../include/cephalopod/actionplayer.hpp"
#include "../include/cephalopod/scene.hpp"

void ceph::ActionPlayer::update(float dt)
{
}

ceph::ActionPlayer::ActionPlayer(Actor& parent) : 
	parent_(parent)
{
}

void ceph::ActionPlayer::run(Scene& scene)
{
	if (isRunning())
		return;

	scene.updateEvent.connect(*this, &ceph::ActionPlayer::update);
}

bool ceph::ActionPlayer::isRunning() const
{
	return parent_.isInScene();
}

bool ceph::ActionPlayer::hasActions() const
{
	return !actions_.empty();
}

void  ceph::ActionPlayer::applyAction(const std::shared_ptr<ceph::Action>& action)
{
	if ( !hasActions() )
		initial_actor_state_ = std::make_unique<ActorState>(parent_);
	actions_.push_back(action);
}

void  ceph::ActionPlayer::applyActions(std::initializer_list<std::shared_ptr<Action>> actions)
{

}

void  ceph::ActionPlayer::removeAction(const std::shared_ptr<ceph::Action>& removee)
{

}