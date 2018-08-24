
#pragma once

#include "types.hpp"

namespace ceph {
	class Actor;

	class ActionConstraint
	{
	public:
		virtual void apply(ceph::Actor& actor) const = 0;
	};

	class WrapTorroidally : public ActionConstraint
	{
	private:
		float horz_padding_;
		float vert_padding_;
	public:
		WrapTorroidally(float horz_padding = 0.0f, float vert_padding = 0.0f);
		virtual void apply(ceph::Actor& actor) const override;
	};
}