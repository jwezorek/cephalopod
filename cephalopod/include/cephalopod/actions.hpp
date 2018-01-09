#pragma once

#include "types.hpp"
#include <functional>

namespace ceph {

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

	Action createMoveByAction(float duration, float x, float y);
	Action createRotateByAction(float duration, float theta);
}

