#include "../include/cephalopod/easingactions.hpp"
#include "easingfunctions.hpp"

void ceph::EasingAction::update(ActorState& state, float t)
{
	child_->update(state, func_(t));
}

ceph::EasingAction::EasingAction(const std::shared_ptr<Action>& child, EasingFnType typ,
		const ceph::EasingFunc& in, const ceph::EasingFunc& out, const ceph::EasingFunc& inout) :
	Action(child->getDuration()),
	child_(child)
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

ceph::BackEasingAction::BackEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Back::in, ceph::ease::Back::out, ceph::ease::Back::inOut) {}

ceph::BounceEasingAction::BounceEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Bounce::in, ceph::ease::Bounce::out, ceph::ease::Bounce::inOut) {}

ceph::CircEasingAction::CircEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Circ::in, ceph::ease::Circ::out, ceph::ease::Circ::inOut) {}

ceph::CubicEasingAction::CubicEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Cubic::in, ceph::ease::Cubic::out, ceph::ease::Cubic::inOut) {}

ceph::ElasticEasingAction::ElasticEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Elastic::in, ceph::ease::Elastic::out, ceph::ease::Elastic::inOut) {}

ceph::ExpoEasingAction::ExpoEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Expo::in, ceph::ease::Expo::out, ceph::ease::Expo::inOut) {}

ceph::QuadEasingAction::QuadEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Quad::in, ceph::ease::Quad::out, ceph::ease::Quad::inOut) {}

ceph::QuartEasingAction::QuartEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Quart::in, ceph::ease::Quart::out, ceph::ease::Quart::inOut) {}

ceph::QuintEasingAction::QuintEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Quint::in, ceph::ease::Quint::out, ceph::ease::Quint::inOut) {}

ceph::SineEasingAction::SineEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::Action>& child) :
	ceph::EasingAction(child, typ, ceph::ease::Sine::in, ceph::ease::Sine::out, ceph::ease::Sine::inOut) {}
