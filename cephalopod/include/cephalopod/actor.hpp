#pragma once

#include <vector>
#include <memory>
#include <initializer_list>
#include "types.hpp"
#include "signals.hpp"
#include "actions.hpp"
#include "actionplayer.hpp"
#include "mat3x3.hpp"
#include "actorstate.hpp"
#include "drawingcontext.hpp"

namespace ceph {

	class ActorImpl;
	class Scene;
	class Action;
	class ActionConstraint;

	class Actor : public Slot<Actor>, public std::enable_shared_from_this<Actor>
	{
		friend class Scene;
		friend class Sprite;
		friend class Group;
		friend class ActorState;
		friend class ActionPlayerImpl;

	private:
		Actor();

	protected:
		ActionPlayer actions_;
		std::weak_ptr<Scene> scene_;
		std::weak_ptr<Actor> parent_;
		std::vector<std::shared_ptr<Actor>> children_;
		ActorState state_;

		virtual void drawThis(DrawingContext& rt) const = 0;
		void detachFromScene();
		void attachToScene(const std::shared_ptr<Scene>& scene);
		void runActions();

	public:

		virtual void initialize() {}

		void addChild(const std::shared_ptr<Actor>&);
		void addChildren(std::initializer_list<std::shared_ptr<Actor>> children);
		void removeChild(const std::shared_ptr<Actor>&);
		void detach();
		bool isInScene() const;
		bool hasParent() const;
		bool isInSceneTopLevel() const;

		ActorState getState() const;

		ActionPlayer& getActions();
		bool hasActions() const;

		std::weak_ptr<Actor> getParent() const;
		std::weak_ptr<Actor> getTopLevelParent() const;
		std::weak_ptr<Scene> getScene() const;

		virtual float getAlpha() const;
		virtual void setAlpha(float alpha);

		virtual float getRotation() const;
		virtual void setRotation(float radians);

		virtual Vec2<float> getScale() const;
		virtual void setScale(float scale);
		virtual void setScale(const Vec2<float>& scale);

		virtual Vec2<float> getPosition() const;
		virtual void setPosition(const Vec2<float>& pt);
		virtual void setPosition(float x, float y);

		virtual Vec2<float> getAnchorPt() const;
		virtual Vec2<float> getAnchorPcnt() const;
		virtual void setAnchorPcnt(const Vec2<float>& pt);
		virtual void setAnchorPcnt(float x, float y);

		virtual Vec2<float> getGlobalPosition() const = 0;
		virtual void setGlobalPosition(const Vec2<float>& pt_global) = 0;
		virtual Rect<float> getLocalBounds() const = 0;
		virtual Rect<float> getGlobalBounds() const = 0;
		virtual Rect<float> getTotalGlobalBounds() const;

		virtual void draw(DrawingContext& rt) const;
		
		virtual ~Actor();

		template<typename A, typename... Args>
		static std::shared_ptr<A> create(Args... construction_args)
		{
			auto a = std::shared_ptr<A>(new A(construction_args...));
			a->initialize();
			return a;
		}

	};

}