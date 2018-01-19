#pragma once

#include "signals.hpp"
#include "actions.hpp"
#include "actionconstraints.hpp"

namespace ceph {
	class ActionPlayer 
	{
	public:
		virtual bool hasActions() const = 0;
		virtual bool isRunning() const = 0;
		virtual void applyAction(int id, const ceph::Action& action, bool repeat = false) = 0;
		virtual void applyAction(const ceph::Action& action, bool repeat = false) = 0;
		virtual void applyActions(std::initializer_list<Action> actions) = 0;
		virtual void applyConstraint(const std::shared_ptr<ActionConstraint>& constraint) = 0;
		virtual void removeAction(int id) = 0;
		virtual Signal<int>& getCompletionSignal(int id) = 0;

		virtual bool hasAction(int id) = 0;
		virtual void clearActions() = 0;
		virtual void clearConstraints() = 0;
		virtual void enforceConstraints() = 0;
		virtual void translateCacheState(const ceph::Vec2D<float> offset) = 0;
	};
}