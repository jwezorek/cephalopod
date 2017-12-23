#pragma once

#include <map>
#include "signals.hpp"
#include "types.hpp"

namespace ceph {

	class Actor;

	struct ActorState
	{
		Vec2D<float> position;
		float scale;
		float rotation;
		float alpha;

		ActorState();
		ActorState(const Actor& actor);
		ActorState& operator+=(Vec2D<float> psotion_offset);
	};

	class Action : public Slot<Action>
	{
		friend class EasingAction;

	private:
		enum class State {
			Detached,
			Running,
			Paused,
			Complete
		};

	protected:
		State state_;
		float duration_;
		bool is_toplevel_;
		std::vector<std::shared_ptr<Action>> children_;

		void run(bool toplevel);
		void setComplete();
		virtual void update(ActorState& state, float t) = 0;

	public:
		Action(float duration);

		void pause();
		void unpause();
		float getDuration() const;
		bool isPaused() const;
		bool isRunning() const;
		bool isToplevel() const;
		bool isComplete() const;

		virtual ~Action();
	};

	class MoveByAction : public Action
	{
	protected:
		Vec2D<float> offset_;
		void update(ActorState& state, float t) override;
	public:
		MoveByAction(float duration, float x, float y);
	};
}

