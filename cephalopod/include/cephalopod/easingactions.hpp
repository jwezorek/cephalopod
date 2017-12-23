#pragma once

#include "actions.hpp"

namespace ceph
{
	enum class EasingFnType
	{
		In,
		Out,
		InOut
	};

	using EasingFunc = std::function<float(float)>;

	class EasingAction : public Action
	{
	protected:
		EasingFunc func_;
		std::shared_ptr<Action> getChild();
		virtual void update(ActorState& state, float t) override;
	public:
		EasingAction(const std::shared_ptr<Action>& child, EasingFnType typ,
			const EasingFunc& in, const EasingFunc& out, const EasingFunc& inout, bool startPaused = false);
	};

	class BackEasingAction : public EasingAction
	{
	public:
		BackEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class BounceEasingAction : public EasingAction
	{
		public:
			BounceEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class CircEasingAction : public EasingAction
	{
	public:
		CircEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class CubicEasingAction : public EasingAction
	{
	public:
		CubicEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class ElasticEasingAction : public EasingAction
	{
	public:
		ElasticEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class ExpoEasingAction : public EasingAction
	{
	public:
		ExpoEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class QuadEasingAction : public EasingAction
	{
	public:
		QuadEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class QuartEasingAction : public EasingAction
	{
	public:
		QuartEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class QuintEasingAction : public EasingAction
	{
	public:
		QuintEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

	class SineEasingAction : public EasingAction
	{
	public:
		SineEasingAction(EasingFnType typ, const std::shared_ptr<Action>& child, bool startPaused = false);
	};

}