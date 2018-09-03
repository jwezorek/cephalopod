#pragma once

#include <memory>

namespace ceph {

	class Scene;
	class DrawingContext;

	class SceneTransition
	{
	protected:

		float elapsed_;
		float duration_;
		std::shared_ptr<Scene> active_scene_;
		std::shared_ptr<Scene> old_scene_;

		virtual void updateTransition(float elapsed);

	public:
		SceneTransition(float duration);
		void setScenes(const std::shared_ptr<Scene>& old_scene, const std::shared_ptr<Scene>& new_scene);
		virtual void update(float elapsed);
		virtual void draw(const DrawingContext& dc) = 0;
		virtual void endGameLoopIteration();
		virtual bool isComplete() const;
	};

	class CrossFadeTransition : public SceneTransition
	{
	public:
		CrossFadeTransition(float duration);
		void draw(const DrawingContext& dc) override;
	};
};