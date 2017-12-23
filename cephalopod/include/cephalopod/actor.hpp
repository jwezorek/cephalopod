#pragma once

#include <vector>
#include <memory>
#include <initializer_list>
#include "types.hpp"
#include "signals.hpp"
#include "actions.hpp"
#include "actionplayer.hpp"

namespace ceph {

	class DrawingContext;
	class ActorImpl;
	class Scene;

	class Actor : public Slot<Actor>, std::enable_shared_from_this<Actor>
	{
		friend class Scene;
		friend class Sprite;
		friend class Group;

	protected:
		std::weak_ptr<Scene> scene_;
		std::weak_ptr<Actor> parent_;
		std::vector<std::shared_ptr<Actor>> children_;
		std::unique_ptr<ActorImpl> impl_;
		ActionPlayer actions_;

		virtual void drawThis(DrawingContext& rt) const = 0;
		void detachFromScene();
		void attachToScene(const std::shared_ptr<Scene>& scene);

	public:
		Actor();

		void addChild(const std::shared_ptr<Actor>&);
		void addChildren(std::initializer_list<std::shared_ptr<Actor>> children);
		void removeChild(const std::shared_ptr<Actor>&);
		void detach();
		bool isInScene() const;
		bool hasParent() const;
		bool isInSceneTopLevel() const;
		bool hasActions() const;

		std::weak_ptr<Actor> getParent() const;
		std::weak_ptr<Actor> getTopLevelParent() const;
		ActionPlayer& getActions();

		virtual float getAlpha() const;
		virtual void setAlpha(float alpha);

		virtual float getRotation() const;
		virtual void setRotation(float radians);

		virtual float getRotationDegrees() const;
		virtual void setRotationDegrees(float degrees);

		virtual float getScale() const;
		virtual void setScale(float scale);

		virtual Point<float> getPosition() const;
		virtual Point<float> getGlobalPosition() const = 0;
		virtual void setGlobalPosition(const Point<float>& pt_global) = 0;
		virtual void setPosition(const Point<float>& pt);
		virtual void setPosition(float x, float y);

		virtual Point<float> getAnchorPt() const;
		virtual void setAnchorPt(const Point<float>& pt);
		virtual void setAnchorPt(float x, float y);

		virtual Rect<float> getLocalBounds() const = 0;
		virtual Rect<float> getGlobalBounds() const = 0;
		virtual Rect<float> getTotalGlobalBounds() const;

		virtual void draw(DrawingContext& rt) const;
		
		virtual ~Actor();
	};

}