#pragma once

#include "types.hpp"
#include <functional>
#include <vector>
#include <iterator>

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

	Action createMoveByAction(float duration, const Vec2<float>& vec);
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

	Action createAnimationAction(std::initializer_list<std::tuple<std::string, float>> frames);
	/*
	Action createAnimationAction(const std::shared_ptr<std::vector<std::tuple<std::string, float>>>& frames);

	template<class It>
	Action createAnimationAction(It first, It last)
	{
		return createAnimationAction(
			std::make_shared<std::vector<std::tuple<std::string, float>>>(first, last)
		);
	}


	Action createAnimationAction(float frame_duration, std::initializer_list<std::string> frames);

	template<class It>
	Action createAnimationAction(float time_step, It first, It last)
	{
		auto tuples = std::make_shared<std::vector<std::tuple<std::string, float>>>();
		std::transform( first, last, std::back_inserter(*tuples),
			[time_step](const std::string& f) {return std::make_tuple(f, time_step); }
		);
		return createAnimationAction(tuples);
	}
	*/
}
