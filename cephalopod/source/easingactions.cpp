#include "../include/cephalopod/easingactions.hpp"
#include "easingfunctions.hpp"

//	std::function<float(float, float, float, float d)> func_;
void ceph::EasingAction::setSpriteState(float elapsed, float timestep)
{
	auto child = getChild();
	auto current = child->getElapsed();
	auto new_elapsed = func_(elapsed + timestep, 0.0f, duration_, duration_);
	auto eased_timestep = new_elapsed - current;
	child->update(eased_timestep);
}

std::shared_ptr<ceph::FiniteAction> ceph::EasingAction::getChild()
{
	return std::static_pointer_cast<ceph::FiniteAction>(children_[0]);
}

ceph::EasingAction::EasingAction(const std::shared_ptr<FiniteAction>& child, EasingFnType typ,
		const ceph::EasingFunc& in, const ceph::EasingFunc& out, const ceph::EasingFunc& inout, bool startPaused) :
	FiniteAction(child->getDuration(), std::vector<std::shared_ptr<ceph::FiniteAction>>{child}, startPaused)
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

/*--------------------------------------------------------------------------------*/

ceph::BounceEasingAction::BounceEasingAction(ceph::EasingFnType typ, const std::shared_ptr<ceph::FiniteAction>& child, bool startPaused) :
	ceph::EasingAction(child, typ, ceph::ease::Bounce::easeIn, ceph::ease::Bounce::easeOut, ceph::ease::Bounce::easeInOut)
{
}
