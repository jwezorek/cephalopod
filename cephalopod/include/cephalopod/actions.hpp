#pragma once

#include "types.hpp"
#include "signals.hpp"

namespace ceph {

	class Actor;

	class ActorState
	{
	private:
		Vec2D<float> translation_;
		float scale_;
		float rotation_;
		float alpha_;

	public:
		ActorState();
		ActorState(const ActorState& actor) = default;
		ActorState(const Actor& actor);
		ActorState& applyTranslation(Vec2D<float> trans);
		Vec2D<float> getTranslation() const;
		float getScale() const;
		float getRotation() const;
		float getAlpha() const;
	};

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
}

