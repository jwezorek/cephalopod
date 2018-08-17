#include <Windows.h>
#include "glad.h"
#include  "GLFW/glfw3.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <array>
#include <iostream>
#include <memory>
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/scene.hpp"
#include "util.hpp"
#include "gameimpl.hpp"
#include "clock.hpp"

namespace
{
	void OnError(int errorCode, const char* msg) {
		throw std::runtime_error(msg);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	GLFWwindow* CreateGlWindow(int wd, int hgt, const char* title)
	{
		// open a window with GLFW
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		auto window = glfwCreateWindow(wd, hgt, title, NULL, NULL);
		if (!window)
			return nullptr;

		glfwSetKeyCallback(window, key_callback);

		// GLFW settings
		glfwMakeContextCurrent(window);

		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		glfwSwapInterval(1);

		return window;
	}

}

ceph::GameImpl::GameImpl() {
	ceph::GameImpl::instance_ = this;

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	// initialise GLFW
	glfwSetErrorCallback(OnError);
	if (!glfwInit())
		throw std::runtime_error("glfwInit failed");
}

ceph::GameImpl* ceph::GameImpl::getInstance()
{
	return instance_;
}



void ceph::GameImpl::initialize(ceph::ScreenMode mode, int wd, int hgt, const std::string& title)
{
	switch (mode) {
		case ceph::ScreenMode::FullScreenExclusive:
			//TODO
			break;
		case ceph::ScreenMode::FullScreenWindowed:
			//TODO
			break;
		case ceph::ScreenMode::WindowedWithTitleBar:
			window_ = CreateGlWindow(wd, hgt, title.c_str());
			break;
	}
	graphics_ = std::make_unique<ceph::Graphics>(window_);
}

void ceph::GameImpl::setLogicalCoordinates(ceph::CoordinateMapping mapping, const ceph::Vec2<float>& log_size, ceph::CoordinateSystem system)
{
	graphics_->setCoordinateSystem(system, mapping, log_size);
}

void ceph::GameImpl::run(const std::shared_ptr<ceph::Scene>& startingScene) {
	active_scene_ = startingScene;

	ceph::Clock clock;
	DrawingContext dc(*graphics_);
	while (!glfwWindowShouldClose(window_)) {
		glfwPollEvents();

		auto elapsed = clock.restart();
		active_scene_->updateActionsEvent.fire(elapsed);
		active_scene_->updateEvent.fire(elapsed);

		graphics_->BeginFrame();
		active_scene_->draw(dc);
		//graphics_->Blit(ceph::Mat3x3().scale(100,100), ceph::Rect<int>(0, 0, 100, 100), 1.0);
		graphics_->EndFrame();
		active_scene_->endGameLoopIteration();

	}
}

ceph::Rect<float> ceph::GameImpl::getLogicalRect() const
{
	return ceph::Rect<float>(0, 0, 0, 0);
}

ceph::Vec2<int> ceph::GameImpl::getScreenSize() const
{
	return ceph::Vec2<int>(0, 0);
}

ceph::Vec2<float> ceph::GameImpl::getLogicalSize() const
{
	return log_size_;
}

void ceph::GameImpl::quit()
{
	
}

ceph::CoordinateMapping ceph::GameImpl::getCoordinateMapping() const
{
	return coord_mapping_mode_;
}


ceph::Rect<float> ceph::GameImpl::getScreenRect() const
{
	int wd, hgt;
	glfwGetWindowSize(window_, &wd, &hgt);
	return ceph::Rect<float>(0.0f, 0.0f, static_cast<float>(wd), static_cast<float>(hgt));
}

ceph::Rect<float> ceph::GameImpl::convertToScreenCoords(const ceph::Rect<float>& rect) const
{
	//TODO
	return ceph::Rect<float>(0, 0, 0, 0);
}

ceph::Rect<float> ceph::GameImpl::convertFromScreenCoords(const ceph::Rect<float>& rect) const
{
	//TODO
	return ceph::Rect<float>(0, 0, 0, 0);
}

ceph::Vec2<float> ceph::GameImpl::convertToScreenCoords(const ceph::Vec2<float>& pt) const
{
	//TODO
	return ceph::Vec2<float>(0, 0);
}

ceph::Vec2<float> ceph::GameImpl::convertFromScreenCoords(const ceph::Vec2<float>& pt) const
{
	//TODO
	return ceph::Vec2<float>(0, 0);
}

std::shared_ptr<ceph::Scene> ceph::GameImpl::getActiveScene() const
{
	return active_scene_;
}

ceph::GameImpl* ceph::GameImpl::instance_ = nullptr;

std::unique_ptr<ceph::Game> ceph::Game::createInstance()
{
	return std::make_unique<GameImpl>();
}

ceph::Game& ceph::Game::getInstance()
{
	auto instance = GameImpl::getInstance();
	if (!instance)
		throw std::runtime_error("No instance of the cephalopod game singleton");
	return *instance;
}



