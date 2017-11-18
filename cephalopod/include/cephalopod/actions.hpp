#pragma once

#include "signals.hpp"

namespace ceph {

	class Action : public Slot<Action>
	{
	private:
		Signal<const Action*> complete_event_;

	public:
		Action();
		virtual ~Action();
	};


}