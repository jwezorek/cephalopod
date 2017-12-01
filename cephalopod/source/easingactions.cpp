#include "../include/cephalopod/easingactions.hpp"
#include "easingfunctions.hpp"

void ceph::EasingAction::doTimeStep(float timestep)
{
	auto child = getChild();

	auto eased_elapsed = func_(elapsed_, 0.0f, duration_, duration_);
	auto new_eased_elapsed = func_(elapsed_ + timestep, 0.0f, duration_, duration_);
	auto eased_timestep = new_eased_elapsed - eased_elapsed;

	child->doTimeStep(eased_timestep);
}

std::shared_ptr<ceph::FiniteAction> ceph::EasingAction::getChild()
{
	return std::static_pointer_cast<ceph::FiniteAction>(children_[0]);
}

ceph::EasingAction::EasingAction(const std::shared_ptr<FiniteAction>& child, EasingFnType typ,
		const ceph::EasingFunc& in, const ceph::EasingFunc& out, const ceph::EasingFunc& inout, bool startPaused) :
	FiniteAction(child->getDuration(), {child}, startPaused)
{
	switch (typ) {
		case EasingFnType::In:
			func_ = in;
			break;
		case EasingFnType::Out:
			func_ = out;
			break;
		case EasingFnType::InOut:
			func_ = inout;
			break;
	}
}

ceph::BackEasingAction::BackEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Back::easeIn, ceph::ease::Back::easeOut, ceph::ease::Back::easeInOut) {}

ceph::BounceEasingAction::BounceEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Bounce::easeIn, ceph::ease::Bounce::easeOut, ceph::ease::Bounce::easeInOut) {}

ceph::CircEasingAction::CircEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Circ::easeIn, ceph::ease::Circ::easeOut, ceph::ease::Circ::easeInOut) {}

ceph::CubicEasingAction::CubicEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Cubic::easeIn, ceph::ease::Cubic::easeOut, ceph::ease::Cubic::easeInOut) {}

ceph::ElasticEasingAction::ElasticEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Elastic::easeIn, ceph::ease::Elastic::easeOut, ceph::ease::Elastic::easeInOut) {}

ceph::ExpoEasingAction::ExpoEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Expo::easeIn, ceph::ease::Expo::easeOut, ceph::ease::Expo::easeInOut) {}

ceph::QuadEasingAction::QuadEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Quad::easeIn, ceph::ease::Quad::easeOut, ceph::ease::Quad::easeInOut) {}

ceph::QuartEasingAction::QuartEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Quart::easeIn, ceph::ease::Quart::easeOut, ceph::ease::Quart::easeInOut) {}

ceph::QuintEasingAction::QuintEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Quint::easeIn, ceph::ease::Quint::easeOut, ceph::ease::Quint::easeInOut) {}

ceph::SineEasingAction::SineEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Sine::easeIn, ceph::ease::Sine::easeOut, ceph::ease::Sine::easeInOut) {}
