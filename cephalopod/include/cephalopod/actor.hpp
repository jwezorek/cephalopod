#pragma once

#include <vector>
#include <memory>
#include "types.hpp"
#include "signals.hpp"

namespace ceph {
	class DrawingContext;
	class ActorImpl;
	class Scene;

	class Actor : public Slot<Actor>, std::enable_shared_from_this<Actor>
	{
		friend class Scene;

	protected:
		std::weak_ptr<Scene> scene_;
		std::weak_ptr<Actor> parent_;
		std::vector<std::shared_ptr<Actor>> children_;
		std::unique_ptr<ActorImpl> impl_;

		virtual void drawThis(DrawingContext& rt) const = 0;
		void detachFromScene();
		void attachToScene(const std::shared_ptr<Scene>& scene);

	public:
		Actor();

		void AddChild(const std::shared_ptr<Actor>&);
		void RemoveChild(const std::shared_ptr<Actor>&);
		void detach();
		bool isInScene() const;
		bool hasParent() const;
		bool isInSceneTopLevel() const;

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

		virtual void draw(DrawingContext& rt) const;
		
		virtual ~Actor();
	};

}