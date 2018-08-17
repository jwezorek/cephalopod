#pragma once

#include <memory>
#include "actor.hpp"
#include "types.hpp"
#include "signals.hpp"
#include "drawingcontext.hpp"

namespace ceph {

	class SpriteImpl;

	class Group : public Actor
	{
		friend class Actor;

	protected:
		void drawThis(DrawingContext& rt) const override {}
		Group();

	public:
		//Vec2<float> getGlobalPosition() const override;
		//void setGlobalPosition(const Vec2<float>& pt_global) override;
		//Rect<float> getLocalBounds() const override;
		//Rect<float> getGlobalBounds() const override;
		virtual ~Group();
	};
}