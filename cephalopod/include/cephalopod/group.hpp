#pragma once

#include <memory>
#include "actor.hpp"
#include "types.hpp"
#include "signals.hpp"
#include "drawingcontext.hpp"

namespace ceph {

	class Group : public Actor
	{
		friend class Actor;

	protected:
		void drawThis(DrawingContext& rt) const override {}
		Group();

	public:
		virtual ~Group();
	};
}