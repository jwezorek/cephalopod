#pragma once

#include "../include/cephalopod/types.hpp"

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
		ActorState& applyRotation(float theta);
		ActorState& applyScale(float scale);
		ActorState& applyAlpha(float alpha);

		Vec2D<float> getTranslation() const;
		float getScale() const;
		float getRotation() const;
		float getAlpha() const;
	};
}