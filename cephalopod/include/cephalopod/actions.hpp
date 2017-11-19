#pragma once

#include "signals.hpp"
#include "actor.hpp"

namespace ceph {

	class Action : public Slot<Action>
	{
	protected:
		std::weak_ptr<Actor> owner_;
		bool is_paused_;
		std::vector<std::shared_ptr<Action>> children_;

	public:
		Action(bool startPaused = false);

		virtual void pause();
		virtual void unpause();
		bool isPaused() const;
		bool hasChildren() const;

		void attach(const std::shared_ptr<Actor> owner);
		void unattach();
		bool isAttached() const;

		virtual void run() = 0;
		virtual void update(float elapsed) = 0;

		~Action();
	};

	class FiniteAction : public Action
	{
	private:
		Signal<const Action*> complete_event_;
		float elapsed_;
		float duration_;
		bool is_complete_;
	public:
		FiniteAction(float duration);

		bool isComplete() const;
		void run() override;
		void update(float elapsed) override;
		void setActionState(float pcnt_complete);
		virtual void setSpriteState(float pcnt_complete) = 0;

	};

}