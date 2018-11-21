#pragma once

#include <memory>
#include "cephalopod/scene.hpp"
#include "cephalopod/label.hpp"
#include "cephalopod/spritesheet.hpp"

class Asteroids;

class IntroScene : public ceph::Scene
{
private:
	std::shared_ptr<ceph::SpriteSheet> sprite_sheet_;
public:
	IntroScene();
	void handleKey(bool isPressed, ceph::KeyCode key, unsigned char modifiers);
};