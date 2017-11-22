#pragma once

#include <map>
#include "signals.hpp"
#include "types.hpp"

namespace ceph {

	class Actor;

	class Action : public Slot<Action>
	{
		friend class Actor;

	protected:
		std::weak_ptr<Actor> owner_;
		bool is_paused_;
		std::vector<std::shared_ptr<Action>> children_;

		virtual void run(const std::shared_ptr<Actor>& actor);
		virtual void stopRunning();
		virtual void update(float elapsed) = 0;

	public:
		Action(bool startPaused = false);

		virtual void pause();
		virtual void unpause();
		bool isPaused() const;
		bool hasChildren() const;
		bool isRunning() const;

		~Action();
	};

	class FiniteAction : public Action
	{
	protected:
		Signal<Action&> complete_event_;
		float elapsed_;
		bool is_complete_;
		float duration_;
		
		virtual void run(const std::shared_ptr<Actor>& actor) override;

	public:
		FiniteAction(float duration, bool startPaused = false);
		void update(float timestep) override;
		template<class T>
		FiniteAction(float duration, const std::vector<std::shared_ptr<T>>& actions, bool startPaused = false) : FiniteAction(duration, startPaused) {
			for (auto action : actions)
				children_.push_back(std::static_pointer_cast<Action>(action));
		}
		float getElapsed() const;
		float getDuration() const;
		bool isComplete() const;
		Signal<Action&>& getCompletionEvent();

		virtual void doTimeStep(float timestep) = 0;
	};

	class MoveByAction : public FiniteAction
	{
	protected:
		Vec2D<float> velocity_;
		void doTimeStep(float timestep) override;

	public:
		MoveByAction(float duration, const Vec2D<float>& amount, bool startPaused = false);
		MoveByAction(float duration, float x, float y, bool startPaused = false);
	};

	class MoveToAction : public MoveByAction
	{
	protected:
		Point<float> dest_;
		void run(const std::shared_ptr<Actor>& actor) override;
	public:
		MoveToAction(float duration, const Point<float>& destination, bool startPaused = false);
	};

	class SetTransparencyToAction : public FiniteAction
	{
	private:
		float alpha_change_rate_;
		float target_alpha_;
	protected:
		virtual void run(const std::shared_ptr<Actor>& actor) override;
		void doTimeStep(float timestep) override;
	public:
		SetTransparencyToAction(float duration, float alpha, bool startPaused = false);
	};

	class SequenceAction : public FiniteAction
	{
	private:
		std::map<float, std::shared_ptr<FiniteAction>> start_tbl_;
		auto getCurrentAction(float elapsed) const;

	protected:
		void doTimeStep(float timestep) override;
	public:
		SequenceAction(const std::vector<std::shared_ptr<FiniteAction>>& actions, bool startPaused = false);
	};
}