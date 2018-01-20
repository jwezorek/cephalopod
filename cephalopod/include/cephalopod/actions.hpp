#pragma once

#include "types.hpp"
#include <functional>
#include <vector>

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

	Action createSimultaneousActions(std::initializer_list<Action> actions);
	Action createSimultaneousActions(const std::shared_ptr<std::vector<Action>>& actions);

	template<class It>
	Action createSimultaneousActions(It first, It last)
	{
		return createSimultaneousActions(
			std::make_shared<std::vector<Action>>(first, last)
		);
	}

	Action createActionSequence(std::initializer_list<Action> actions);
	Action createActionSequence(const std::shared_ptr<std::vector<Action>>& actions);

	template<class It>
	Action createActionSequence(It first, It last)
	{
		return createActionSequence(
			std::make_shared<std::vector<Action>>(first, last)
		);
	}
}

