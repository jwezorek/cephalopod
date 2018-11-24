#include "../include/cephalopod/scenetransition.hpp"
#include "../include/cephalopod/scene.hpp"

ceph::SceneTransition::SceneTransition(float duration) : 
	duration_(duration), elapsed_(0), old_scene_(nullptr), active_scene_(nullptr)
{
}

void ceph::SceneTransition::setScenes(Scene& old_scene, Scene& new_scene)
{
	old_scene_ = &old_scene;
	active_scene_ = &new_scene;
}

void ceph::SceneTransition::update(float elapsed)
{
	old_scene_->update( elapsed );
	active_scene_->update(elapsed);
	updateTransition(elapsed);
}

void ceph::SceneTransition::endGameLoopIteration()
{
	old_scene_->endGameLoopIteration();
	active_scene_->endGameLoopIteration();
}

bool ceph::SceneTransition::isComplete() const
{
	return elapsed_ >= duration_;
}

void ceph::SceneTransition::updateTransition(float dt)
{
	elapsed_ += dt;
	elapsed_ = (isComplete()) ? duration_ : elapsed_;
}

/*---------------------------------------------------------------------------------------------------------------------*/

ceph::CrossFadeTransition::CrossFadeTransition(float duration) : ceph::SceneTransition(duration)
{
}

void ceph::CrossFadeTransition::draw(const DrawingContext& dc)
{
	float new_alpha = elapsed_ / duration_;
	float old_alpha = 1.0f - new_alpha;

	DrawingContext dc_new(dc);
	dc_new.alpha *= new_alpha;
	active_scene_->draw(dc_new);

	DrawingContext dc_old(dc);
	dc_old.alpha *= old_alpha;
	old_scene_->draw(dc_old);
}
