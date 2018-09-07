#pragma once

#include <memory>

namespace ceph {

	class Scene;
	struct DrawingContext;

	class SceneTransition
	{
	protected:

		float elapsed_;
		float duration_;
		Scene* active_scene_;
		Scene* old_scene_;

		virtual void updateTransition(float elapsed);

	public:
		SceneTransition(float duration);
		void setScenes(Scene& old_scene, Scene& new_scene);
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