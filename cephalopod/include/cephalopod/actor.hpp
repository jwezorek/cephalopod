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
		friend class ActionPlayer;

	private:
		Actor();

	protected:
		ActionPlayer actions_;
		std::vector<std::shared_ptr<ActionConstraint>> constraints_;
		Scene* scene_;
		std::weak_ptr<Actor> parent_;
		std::vector<std::shared_ptr<Actor>> children_;
		ActorState state_;

		virtual void drawThis(DrawingContext& rt) const = 0;
		void detachFromScene();
		void attachToScene(Scene& scene);
		void runActions();
		void setActorState(const ActorState& state);

		Mat3x3 getLocalToGlobalTransform() const; 
		Mat3x3 getGlobalToLocalTransform() const;

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
		Scene& getScene() const;

		float getAlpha() const;
		void setAlphaTo(float alpha);
		void changeAlphaBy(float alpha);

		float getRotation() const;
		void rotateTo(float radians);
		void rotateBy(float radians);

		Vec2<float> getScale() const;
		void setScaleTo(float scale);
		void setScaleTo(const Vec2<float>& scale);
		void changeScaleBy(float scale);
		void changeScaleBy(const Vec2<float>& scale);

		Vec2<float> getPosition() const;
		void moveTo(const Vec2<float>& pt);
		void moveTo(float x, float y);
		void moveBy(const Vec2<float>& pt);
		void moveBy(float x, float y);

		Vec2<float> getAnchorPt() const;
		Vec2<float> getAnchorPcnt() const;
		void setAnchorPcnt(const Vec2<float>& pt);
		void setAnchorPcnt(float x, float y);

		Vec2<float> getGlobalPosition() const;
		void setGlobalPosition(const Vec2<float>& pt_global);

		Rect<float> getLocalBounds() const;
		Rect<float> getGlobalBounds() const;
		Rect<float> getTotalGlobalBounds() const;

		virtual void applyConstraint(const std::shared_ptr<ActionConstraint>& constraint);
		void clearConstraints();
		void enforceConstraints();

		void draw(DrawingContext& rt) const;
		
		~Actor();

		template<typename A, typename... Args>
		static std::shared_ptr<A> create(Args... construction_args)
		{
			auto a = std::shared_ptr<A>(new A(construction_args...));
			a->initialize();
			return a;
		}

	};

}