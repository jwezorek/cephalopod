#pragma once

#include "cephalopod/scene.hpp"
#include "cephalopod/button.hpp"

class IntroScene : public ceph::Scene
{
private:
	std::shared_ptr<ceph::SpriteSheet> sprite_sheet_;
	std::shared_ptr<ceph::Button> start_btn_;
	std::shared_ptr<ceph::Button> exit_btn_;
public:
	IntroScene();
	void handleKey(bool isPressed, ceph::KeyCode key, ceph::KeyModifiers modifiers);
};