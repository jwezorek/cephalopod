#pragma once

#include <memory>
#include "actor.hpp"
#include "types.hpp"
#include "signals.hpp"

namespace ceph {

	class SpriteImpl;

	class Group : public Actor
	{
	protected:
		std::unique_ptr<SpriteImpl> impl_;
		void drawThis(DrawingContext& rt) const override {}
	public:
		Group();
		Point<float> getGlobalPosition() const override;
		void setGlobalPosition(const Point<float>& pt_global) override;
		Rect<float> getLocalBounds() const override;
		Rect<float> getGlobalBounds() const override;
	};
}