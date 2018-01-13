#pragma once

#include "../include/cephalopod/signals.hpp"
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actionconstraints.hpp"

namespace ceph {

	class Scene;

	class ActionPlayer : public Slot<ActionPlayer>
	{
		friend Scene;
		friend Actor;

	private:

		struct ActionInProgress
		{
			int id;
			Action action;
			float elapsed;
			Signal<Action&> signal;
			bool complete;
			bool repeat;

			// only moveable due to signal
			ActionInProgress(const ActionInProgress&) = delete;
			ActionInProgress& operator=(const ActionInProgress&) = delete;
			ActionInProgress(ActionInProgress&& a) = default;
			ActionInProgress& operator=(ActionInProgress&& a) = default;

			ActionInProgress(int id, const Action& a, bool rep); 
			float getPcntComplete() const;
		};

		Actor& parent_;
		std::unique_ptr<ActorState> initial_actor_state_;
		std::vector<ActionInProgress> actions_;
		std::vector<std::shared_ptr<ActionConstraint>> constraints_;

		void removeActions(const std::function<bool(const ActionInProgress&)> predicate);
		void finalizeAction(ceph::ActionPlayer::ActionInProgress & aip, bool emit_signal = true);
		void resetActions();
		void update(float dt);
		void setActorState(const ActorState& state);
		void applyConstraints(ActorState& state);
		void run();

	public:
		ActionPlayer(Actor& parent);
		bool hasActions() const;
		bool isRunning() const;
		void applyAction(int id, const ceph::Action& action, bool repeat = false);
		void applyAction(const ceph::Action& action, bool repeat = false);
		void applyActions(std::initializer_list<Action> actions);
		void applyConstraint(const std::shared_ptr<ActionConstraint>& constraint);
		void removeAction(int id);
		void clearActions();
		void translateCacheState(const ceph::Vec2D<float> offset);
	};

}