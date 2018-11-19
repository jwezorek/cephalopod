#pragma once

#include <memory>
#include "cephalopod/scene.hpp"
#include "cephalopod/label.hpp"

class Asteroids;

class IntroScene : public ceph::Scene
{
private:
	std::shared_ptr<Asteroids> asteroids_;
	std::shared_ptr<ceph::Label> label_;
public:
	IntroScene();
	void setMainScene(const std::shared_ptr<Asteroids>& a); 
	void handleKey(bool isPressed, ceph::KeyCode key, unsigned char modifiers);
};