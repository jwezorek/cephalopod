#pragma once

#include "types.hpp"
#include "signals.hpp"

namespace ceph {

	class Actor;
	class ActorState;

	class Action : public Slot<Action>
	{
		friend class EasingAction;
		friend class ActionPlayer;

	protected:
		float duration_;
		virtual void update(ActorState& state, float t) const = 0;

	public:
		Action(float duration);
		float getDuration() const;
		virtual ~Action();
	};

	class MoveByAction : public Action
	{
	protected:
		Vec2D<float> offset_;
		void update(ActorState& state, float t) const override;
	public:
		MoveByAction(float duration, float x, float y);
	};

	class RotateByAction : public Action
	{
	protected:
		float theta_;
		void update(ActorState& state, float t) const override;
	public:
		RotateByAction(float duration, float theta);
	};
}

