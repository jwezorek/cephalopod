#pragma once

#include "types.hpp"

namespace ceph {
	class Actor;
	class ActorState;

	class ActionConstraint
	{
	public:
		virtual void apply(ceph::Actor& actor) const = 0;
	};

	class WrapTorroidally : public ActionConstraint
	{
	public:
		virtual void apply(ceph::Actor& actor) const;
	};
}