#pragma once

#include "signals.hpp"
#include "types.hpp"

namespace ceph {

	class Actor;

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

		virtual void run(const std::shared_ptr<Actor>& actor);
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
		FiniteAction(float duration, bool startPaused = false);

		bool isComplete() const;
		void update(float elapsed) override;
		void setActionState(float pcnt_complete);
		virtual void run(const std::shared_ptr<Actor>& actor) override;
		virtual void setSpriteState(float pcnt_complete) = 0;

	};

	class MoveToAction : public FiniteAction
	{
	private:
		Point<float> start_;
		Point<float> dest_;
	public: 
		MoveToAction(float duration, const Point<float>& dest, bool startPaused = false);
		void run(const std::shared_ptr<Actor>& actor) override;
		void setSpriteState(float pcnt_complete) override;
	};

}