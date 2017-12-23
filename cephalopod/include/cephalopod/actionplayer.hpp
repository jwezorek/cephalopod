#pragma once
#include "actions.hpp"

namespace ceph {

	class Scene;

	class ActionPlayer : public Slot<ActionPlayer>
	{
	private:
		Actor& parent_;
		std::unique_ptr<ActorState> initial_actor_state_;
		std::vector<std::shared_ptr<Action>> actions_;

		void update(float dt);

	public:
		ActionPlayer(Actor& parent);
		void run(Scene& scene);
		bool hasActions() const;
		bool isRunning() const;
		void applyAction(const std::shared_ptr<ceph::Action>& action);
		void applyActions(std::initializer_list<std::shared_ptr<Action>> actions);
		void removeAction(const std::shared_ptr<ceph::Action>& removee);
	};

}