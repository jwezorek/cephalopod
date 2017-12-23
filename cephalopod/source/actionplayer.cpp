#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actionplayer.hpp"

void ceph::ActionPlayer::update(float dt)
{
}

void ceph::ActionPlayer::run(Scene& scene)
{
}

bool ceph::ActionPlayer::hasActions() const
{
	return false;
}

void  ceph::ActionPlayer::applyAction(const std::shared_ptr<ceph::Action>& action)
{

}

void  ceph::ActionPlayer::applyActions(std::initializer_list<std::shared_ptr<Action>> actions)
{

}

void  ceph::ActionPlayer::removeAction(const std::shared_ptr<ceph::Action>& removee)
{

}