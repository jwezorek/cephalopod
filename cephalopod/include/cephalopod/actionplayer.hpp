
#pragma once

#include "signals.hpp"
#include "actions.hpp"
#include "actionconstraints.hpp"

namespace ceph {
	class ActionPlayer : public Slot<ActionPlayer>
	{
		friend Actor;

	private:

		struct ActionInProgress
		{
			int id;
			Action action;
			float elapsed;
			Signal<int> signal;
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

		void removeActions(const std::function<bool(const ActionInProgress&)> predicate);
		void finalizeAction(ActionInProgress & aip, bool emit_signal = true);
		void resetActions();
		void update(float dt);
		void run();

		void moveBy(const Vec2<float>& vec);
		void rotateBy(float theta);
		void changeAlphaBy(float alpha);
		void changeScaleBy(const Vec2<float>& scale);

	public:
		ActionPlayer(Actor& parent);

		bool hasActions() const;
		bool isRunning() const;
		void applyAction(int id, const ceph::Action& action, bool repeat = false);
		void applyAction(const ceph::Action& action, bool repeat = false);
		void applyActions(std::initializer_list<Action> actions);
		void removeAction(int id);
		Signal<int>& getCompletionSignal(int id);

		bool hasAction(int id);
		void clearActions();
	};
}