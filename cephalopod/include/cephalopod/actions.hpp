#pragma once

#include "types.hpp"
#include <functional>

namespace ceph {

	class Actor;
	class ActorState;
	using ActionFunction = std::function<void(ActorState&, float)>;

	class Action 
	{

	private:
		float duration_;
		ActionFunction function_;
		
	public:
		Action(float d, ActionFunction f);
		float getDuration() const;
		ActionFunction getFunction() const;
		void operator()(ActorState& state, float t) const;
	};

	Action createMoveByAction(float duration, const Vec2D<float>& vec);
	Action createMoveByAction(float duration, float x, float y);
	Action createRotateByAction(float duration, float theta);
	Action createFadeByAction(float duration, float alpha);
	Action createFadeOutAction(float duration, const Actor& actor);
}

