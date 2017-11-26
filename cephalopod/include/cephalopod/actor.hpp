#pragma once

#include <vector>
#include <memory>
#include "types.hpp"
#include "signals.hpp"
#include "actions.hpp"

namespace ceph {

	class DrawingContext;
	class ActorImpl;
	class Scene;

	class Actor : public Slot<Actor>, std::enable_shared_from_this<Actor>
	{
		friend class Scene;
		friend class Sprite;

	protected:
		std::weak_ptr<Scene> scene_;
		std::weak_ptr<Actor> parent_;
		std::vector<std::shared_ptr<Actor>> children_;
		std::unique_ptr<ActorImpl> impl_;
		std::vector<std::shared_ptr<Action>> actions_;

		virtual void drawThis(DrawingContext& rt) const = 0;
		void detachFromScene();
		void attachToScene(const std::shared_ptr<Scene>& scene);
		void runAction(const std::shared_ptr<Action>& action);
		void runActions();

	public:
		Actor();

		void addChild(const std::shared_ptr<Actor>&);
		void removeChild(const std::shared_ptr<Actor>&);
		void detach();
		bool isInScene() const;
		bool hasParent() const;
		bool isInSceneTopLevel() const;

		std::weak_ptr<Actor> getParent() const;
		std::weak_ptr<Actor> getTopLevelParent() const;

		bool hasActions() const;
		void applyAction(const std::shared_ptr<Action>& action);
		void removeAction(const std::shared_ptr<Action>& action);

		virtual float getAlpha() const;
		virtual void setAlpha(float alpha);

		virtual float getRotation() const;
		virtual void setRotation(float radians);

		virtual float getRotationDegrees() const;
		virtual void setRotationDegrees(float degrees);

		virtual float getScale() const;
		virtual void setScale(float radians);

		virtual Point<float> getPosition() const;
		virtual void setPosition(const Point<float>& pt);
		virtual void setPosition(float x, float y);

		virtual Point<float> getAnchorPt() const;
		virtual void setAnchorPt(const Point<float>& pt);
		virtual void setAnchorPt(float x, float y);

		virtual ceph::Rect<float> getLocalBounds() const = 0;
		virtual ceph::Rect<float> getGlobalBounds() const = 0;
		virtual ceph::Rect<float> getTotalGlobalBounds() const;

		virtual void draw(DrawingContext& rt) const;
		
		virtual ~Actor();
	};

}