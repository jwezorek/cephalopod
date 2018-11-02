#pragma once

#include "actions.hpp"

namespace ceph {
	enum class EasingFunctionType
	{
		BackFirst,
		Bouncy,
		Circular,
		Cubic,
		Elastic,
		Exponential,
		Quadratic,
		Quartic,
		Quintic,
		Sinusoidal
	};

	enum class EasingType
	{
		In,
		Out,
		InOut
	};

	template<EasingFunctionType F, EasingType E>
	Action createEasingAction(const Action& child_action);
}