#pragma once

#include <memory>
#include "cephalopod/scene.hpp"

class Asteroids;

class IntroScene : public ceph::Scene
{
private:
	std::shared_ptr<Asteroids> asteroids_;

public:
	void setMainScene(const std::shared_ptr<Asteroids>& a); 
	void handleKey(bool isPressed, ceph::KeyCode key, unsigned char modifiers);
	void initialize() override;
};