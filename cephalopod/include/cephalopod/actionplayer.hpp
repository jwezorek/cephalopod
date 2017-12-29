#pragma once

#include "signals.hpp"
#include "actions.hpp"
#include "actionconstraints.hpp"

namespace ceph {

	class Scene;

	class ActionPlayer : public Slot<ActionPlayer>
	{
		friend Scene;
		friend Actor;

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

			ActionInProgress(std::shared_ptr<Action> a, bool rep) : 
				action(a), 
				elapsed(0.0f), 
				complete(false), 
				repeat(rep)
			{}
		};

		Actor& parent_;
		std::unique_ptr<ActorState> initial_actor_state_;
		std::vector<ActionInProgress> actions_;
		std::vector<std::shared_ptr<ActionConstraint>> constraints_;

		void markActionAsComplete(ceph::ActionPlayer::ActionInProgress & aip);
		void resetActions();
		void update(float dt);
		void setActorState(const ActorState& state);
		void applyConstraints(Actor& actor, const ActorState& prevState);
		void run();

	public:
		ActionPlayer(Actor& parent);
		bool hasActions() const;
		bool isRunning() const;
		void applyAction(const std::shared_ptr<ceph::Action>& action, bool repeat = false);
		void applyActions(std::initializer_list<std::shared_ptr<Action>> actions);
		void applyConstraint(const std::shared_ptr<ActionConstraint>& constraint);
		void removeAction(const std::shared_ptr<ceph::Action>& removee);
	};

}