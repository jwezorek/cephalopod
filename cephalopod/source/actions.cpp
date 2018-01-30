#define _USE_MATH_DEFINES
#include <cmath>
#include "../include/cephalopod/actions.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/types.hpp"
#include "actorstate.hpp"
#include "util.hpp"
#include <map>
#include <numeric>

ceph::Action::Action(float d, ActionFunction f) : duration_(d), function_(f)
{}

float ceph::Action::getDuration() const
{
	return duration_;
}

ceph::ActionFunction ceph::Action::getFunction() const
{
	return function_;
}

void ceph::Action::operator()(ceph::ActorState & state, float t) const
{
	function_(state, t);
}

ceph::Action ceph::createMoveByAction(float duration, const ceph::Vec2D<float>& vec)
{
	return ceph::createMoveByAction(duration, vec.x, vec.y);
}

ceph::Action ceph::createMoveByAction(float duration, float x, float y)
{
	return ceph::Action(
		duration,
		[x, y](ceph::ActorState& state, float t) -> void {
			state.translate( t*x, t*y);
		}
	);
}

ceph::Action ceph::createRotateByAction(float duration, float theta)
{
	return ceph::Action(
		duration,
		[theta](ceph::ActorState& state, float t) -> void {
			state.rotate( t*theta );
		}
	);
}

ceph::Action ceph::createFadeByAction(float duration, float alpha)
{
	return ceph::Action(
		duration,
		[alpha](ceph::ActorState& state, float t) -> void {
			float old_alpha = state.getAlpha();
			state.setAlpha( old_alpha + t*alpha);
		}
	);
}

ceph::Action ceph::createFadeOutAction(float duration, const ceph::Actor& actor)
{
	return ceph::createFadeByAction(duration, -actor.getAlpha());
}

ceph::Action ceph::createSimultaneousActions(std::initializer_list<ceph::Action> actions)
{
	return ceph::createSimultaneousActions(actions.begin(), actions.end());
}

ceph::Action ceph::createSimultaneousActions(const std::shared_ptr<std::vector<ceph::Action>>& actions)
{
	float duration = std::max_element (
			actions->begin(), actions->end(),
			[](auto& a1, auto& a2) { return a1.getDuration() < a2.getDuration(); }
		)->getDuration(); 

	return ceph::Action(
		duration,
		[actions, duration](ceph::ActorState& state, float t) -> void {
			for (auto action : *actions) {
				float action_t = (t * duration) / action.getDuration();
				action_t = (action_t <= 1.0f) ? action_t : 1.0f;
				action(state, action_t);
			}
		}
	);
}

ceph::Action ceph::createActionSequence(std::initializer_list<ceph::Action> actions)
{
	return ceph::createActionSequence(actions.begin(), actions.end());
}

ceph::Action ceph::createActionSequence(const std::shared_ptr<std::vector<ceph::Action>>& actions)
{
	float duration = std::accumulate(
		actions->begin(), actions->end(), 0.0f,
		[](float sum, ceph::Action& a)->float { return sum + a.getDuration(); }
	);
	return ceph::Action(
		duration,
		[actions, duration](ceph::ActorState& state, float t) -> void {
				float time = t * duration;
				float action_start = 0.0f;
				for (auto action : *actions) {
					float action_end = action_start + action.getDuration();
					float action_t = 0.0f;
					if (time >= action_start && time <= action_end)
						action_t = (time - action_start) / action.getDuration();
					else if (time > action_end)
						action_t = 1.0f;
					else if (time < action_start)
						return;
					action(state, action_t);
					action_start += action.getDuration();
				}
			}
		);
}

ceph::Action ceph::createAnimationAction(std::initializer_list<std::tuple<std::string, float>> frames)
{
	return ceph::createAnimationAction(frames.begin(), frames.end());
}

ceph::Action ceph::createAnimationAction(const std::shared_ptr<std::vector<std::tuple<std::string, float>>>& frames)
{
	float duration = std::accumulate(
		frames->begin(), frames->end(), 0.0f,
		[](float sum, std::tuple<std::string, float>& pair)->float { return sum + std::get<1>(pair); }
	);
	return ceph::Action(
			duration,
			[frames, duration](ceph::ActorState& state, float t) -> void {
			float time = t * duration;
			float frame_start = 0.0f;
			std::string frame_name;
			for (auto frame_pair : *frames) {
				float frame_duration = std::get<1>(frame_pair);
				float frame_end = frame_start + frame_duration;

				if (time >= frame_start && time <= frame_end)
					frame_name = std::get<0>(frame_pair);

				frame_start += frame_duration;
			}
			state.setSpriteFrame(frame_name);
		}
	);
}

ceph::Action ceph::createAnimationAction(float frame_duration, std::initializer_list<std::string> frames)
{
	return ceph::createAnimationAction(frame_duration, frames.begin(), frames.end());
}
