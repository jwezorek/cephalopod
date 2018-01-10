#include "../include/cephalopod/easingactions.hpp"
#include "easingfunctions.hpp"

namespace
{
	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::BackFirst && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Back::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::BackFirst && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Back::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::BackFirst && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Back::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Bouncy && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Bounce::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Bouncy && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Bounce::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Bouncy && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Bounce::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Circular && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Circ::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Circular && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Circ::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Circular && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Circ::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Cubic && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Cubic::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Cubic && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Cubic::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Cubic && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Cubic::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Elastic && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Elastic::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Elastic && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Elastic::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Elastic && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Elastic::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Exponential && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Expo::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Exponential && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Expo::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Exponential && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Expo::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quadratic && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quad::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quadratic && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quad::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quadratic && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quad::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quartic && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quart::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quartic && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quart::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quartic && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quart::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quintic && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quint::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quintic && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quint::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Quintic && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Quint::out(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Sinusoidal && E == ceph::EasingType::In), int>::type D>
	float easingFunction(float t) { return ceph::ease::Sine::in(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Sinusoidal && E == ceph::EasingType::InOut), int>::type D>
	float easingFunction(float t) { return ceph::ease::Sine::inOut(t); }

	template<ceph::EasingFunctionType F, ceph::EasingType E, typename std::enable_if<(F == ceph::EasingFunctionType::Sinusoidal && E == ceph::EasingType::Out), int>::type D>
	float easingFunction(float t) { return ceph::ease::Sine::out(t); }
}

template<ceph::EasingFunctionType F, ceph::EasingType E>
ceph::Action ceph::createEasingAction(const ceph::Action& child_action)
{
	return ceph::Action(
		child_action.getDuration(),
		[child_action](ceph::ActorState& state, float t) -> void {
			child_action( state, easingFunction<F, E, 0>(t) );
		}
	);
}

template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::BackFirst, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::BackFirst, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::BackFirst, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Bouncy, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Bouncy, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Bouncy, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Circular, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Circular, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Circular, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Cubic, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Cubic, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Cubic, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Elastic, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Elastic, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Elastic, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Exponential, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Exponential, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Exponential, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quadratic, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quadratic, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quadratic, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quartic, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quartic, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quartic, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quintic, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quintic, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Quintic, ceph::EasingType::Out>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Sinusoidal, ceph::EasingType::In>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Sinusoidal, ceph::EasingType::InOut>(const ceph::Action&);
template ceph::Action ceph::createEasingAction<ceph::EasingFunctionType::Sinusoidal, ceph::EasingType::Out>(const ceph::Action&);
