#pragma once

#include "actions.hpp"

namespace ceph
{
	enum class EasingFnType
	{
		In,
		Out,
		InOut
	};

	using EasingFunc = std::function<float(float, float, float, float d)>;

	class EasingAction : public FiniteAction
	{
	protected:
		EasingFunc func_;
		std::shared_ptr<FiniteAction> getChild();
		virtual void doTimeStep(float timestep) override;
	public:
		EasingAction(const std::shared_ptr<FiniteAction>& child, EasingFnType typ,
			const EasingFunc& in, const EasingFunc& out, const EasingFunc& inout, bool startPaused = false);
	};

	class BounceEasingAction : public EasingAction
	{
		public:
			BounceEasingAction(EasingFnType typ, const std::shared_ptr<FiniteAction>& child, bool startPaused = false);
	};
}