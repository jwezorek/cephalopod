#pragma once

#include <algorithm>
#include "signals.hpp"
#include "actions.hpp"

namespace ceph {

	class Scene;

	class ActionPlayer : public Slot<ActionPlayer>
	{
		friend Scene;

	private:

		struct ActionInProgress
		{
			std::shared_ptr<Action> action;
			float elapsed;
			Signal<Action&> signal;
			bool complete;
			bool repeat;

			// only moveable due to signal
			ActionInProgress(const ActionInProgress&) = delete;
			ActionInProgress& operator=(const ActionInProgress&) = delete;
			ActionInProgress(ActionInProgress&& a) = default;
			ActionInProgress& operator=(ActionInProgress&& a) = default;

			ActionInProgress(std::shared_ptr<Action> a) : 
				action(a), 
				elapsed(0.0f), 
				complete(false), 
				repeat(false)
			{}
		};

		Actor& parent_;
		std::unique_ptr<ActorState> initial_actor_state_;
		std::vector<ActionInProgress> actions_;

		void update(float dt);
		void setActorState(const ActorState& state);
		void run();

	public:
		ActionPlayer(Actor& parent);
		bool hasActions() const;
		bool isRunning() const;
		void applyAction(const std::shared_ptr<ceph::Action>& action);
		void applyActions(std::initializer_list<std::shared_ptr<Action>> actions);
		void removeAction(const std::shared_ptr<ceph::Action>& removee);
	};

}