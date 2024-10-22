#define NOMINMAX
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
#include "../include/cephalopod/scenetransition.hpp"
#include "graphics.hpp"
#include "util.hpp"
#include "clock.hpp"

//#include "spritepacker.hpp"

namespace {
	ceph::Game* g_instance_ = nullptr;

	static std::unordered_map<int, ceph::KeyCode> map_glfw_to_ceph_key = {
			{ GLFW_KEY_SPACE, ceph::KeyCode::Space },
			{ GLFW_KEY_APOSTROPHE, ceph::KeyCode::Apostrophe },
			{ GLFW_KEY_COMMA, ceph::KeyCode::Comma },
			{ GLFW_KEY_MINUS, ceph::KeyCode::Dash },
			{ GLFW_KEY_PERIOD, ceph::KeyCode::Period },
			{ GLFW_KEY_SLASH, ceph::KeyCode::Slash },
			{ GLFW_KEY_0, ceph::KeyCode::Num0 },
			{ GLFW_KEY_1, ceph::KeyCode::Num1 },
			{ GLFW_KEY_2, ceph::KeyCode::Num2 },
			{ GLFW_KEY_3, ceph::KeyCode::Num3 },
			{ GLFW_KEY_4, ceph::KeyCode::Num4 },
			{ GLFW_KEY_5, ceph::KeyCode::Num5 },
			{ GLFW_KEY_6, ceph::KeyCode::Num6 },
			{ GLFW_KEY_7, ceph::KeyCode::Num7 },
			{ GLFW_KEY_8, ceph::KeyCode::Num8 },
			{ GLFW_KEY_9, ceph::KeyCode::Num9 },
			{ GLFW_KEY_SEMICOLON, ceph::KeyCode::SemiColon },
			{ GLFW_KEY_EQUAL, ceph::KeyCode::Equal },
			{ GLFW_KEY_A, ceph::KeyCode::A },
			{ GLFW_KEY_B, ceph::KeyCode::B },
			{ GLFW_KEY_C, ceph::KeyCode::C },
			{ GLFW_KEY_D, ceph::KeyCode::D },
			{ GLFW_KEY_E, ceph::KeyCode::E },
			{ GLFW_KEY_F, ceph::KeyCode::F },
			{ GLFW_KEY_G, ceph::KeyCode::G },
			{ GLFW_KEY_H, ceph::KeyCode::H },
			{ GLFW_KEY_I, ceph::KeyCode::I },
			{ GLFW_KEY_J, ceph::KeyCode::J },
			{ GLFW_KEY_K, ceph::KeyCode::K },
			{ GLFW_KEY_L, ceph::KeyCode::L },
			{ GLFW_KEY_M, ceph::KeyCode::M },
			{ GLFW_KEY_N, ceph::KeyCode::N },
			{ GLFW_KEY_O, ceph::KeyCode::O },
			{ GLFW_KEY_P, ceph::KeyCode::P },
			{ GLFW_KEY_Q, ceph::KeyCode::Q },
			{ GLFW_KEY_R, ceph::KeyCode::R },
			{ GLFW_KEY_S, ceph::KeyCode::S },
			{ GLFW_KEY_T, ceph::KeyCode::T },
			{ GLFW_KEY_U, ceph::KeyCode::U },
			{ GLFW_KEY_V, ceph::KeyCode::V },
			{ GLFW_KEY_W, ceph::KeyCode::W },
			{ GLFW_KEY_X, ceph::KeyCode::X },
			{ GLFW_KEY_Y, ceph::KeyCode::Y },
			{ GLFW_KEY_Z, ceph::KeyCode::Z },
			{ GLFW_KEY_LEFT_BRACKET, ceph::KeyCode::LBracket },
			{ GLFW_KEY_BACKSLASH, ceph::KeyCode::BackSlash },
			{ GLFW_KEY_RIGHT_BRACKET, ceph::KeyCode::RBracket },
			{ GLFW_KEY_ESCAPE, ceph::KeyCode::Escape },
			{ GLFW_KEY_ENTER, ceph::KeyCode::Enter },
			{ GLFW_KEY_TAB, ceph::KeyCode::Tab },
			{ GLFW_KEY_BACKSPACE, ceph::KeyCode::BackSpace },
			{ GLFW_KEY_INSERT, ceph::KeyCode::Insert },
			{ GLFW_KEY_DELETE, ceph::KeyCode::Delete },
			{ GLFW_KEY_RIGHT, ceph::KeyCode::Right },
			{ GLFW_KEY_LEFT, ceph::KeyCode::Left },
			{ GLFW_KEY_DOWN, ceph::KeyCode::Down },
			{ GLFW_KEY_UP, ceph::KeyCode::Up },
			{ GLFW_KEY_PAGE_UP, ceph::KeyCode::PageUp },
			{ GLFW_KEY_PAGE_DOWN, ceph::KeyCode::PageDown },
			{ GLFW_KEY_HOME, ceph::KeyCode::Home },
			{ GLFW_KEY_END, ceph::KeyCode::End },
			{ GLFW_KEY_CAPS_LOCK, ceph::KeyCode::CapsLock },
			{ GLFW_KEY_SCROLL_LOCK, ceph::KeyCode::ScrollLock },
			{ GLFW_KEY_NUM_LOCK, ceph::KeyCode::NumLock },
			{ GLFW_KEY_PRINT_SCREEN, ceph::KeyCode::PrintScreen },
			{ GLFW_KEY_PAUSE, ceph::KeyCode::Pause },
			{ GLFW_KEY_F1, ceph::KeyCode::F1 },
			{ GLFW_KEY_F2, ceph::KeyCode::F2 },
			{ GLFW_KEY_F3, ceph::KeyCode::F3 },
			{ GLFW_KEY_F4, ceph::KeyCode::F4 },
			{ GLFW_KEY_F5, ceph::KeyCode::F5 },
			{ GLFW_KEY_F6, ceph::KeyCode::F6 },
			{ GLFW_KEY_F7, ceph::KeyCode::F7 },
			{ GLFW_KEY_F8, ceph::KeyCode::F8 },
			{ GLFW_KEY_F9, ceph::KeyCode::F9 },
			{ GLFW_KEY_F10, ceph::KeyCode::F10},
			{ GLFW_KEY_F11, ceph::KeyCode::F11},
			{ GLFW_KEY_F12, ceph::KeyCode::F12},
			{ GLFW_KEY_F13, ceph::KeyCode::F13},
			{ GLFW_KEY_F14, ceph::KeyCode::F14},
			{ GLFW_KEY_F15, ceph::KeyCode::F15},
			{ GLFW_KEY_KP_0, ceph::KeyCode::Numpad0 },
			{ GLFW_KEY_KP_1, ceph::KeyCode::Numpad1 },
			{ GLFW_KEY_KP_2, ceph::KeyCode::Numpad2 },
			{ GLFW_KEY_KP_3, ceph::KeyCode::Numpad3 },
			{ GLFW_KEY_KP_4, ceph::KeyCode::Numpad4 },
			{ GLFW_KEY_KP_5, ceph::KeyCode::Numpad5 },
			{ GLFW_KEY_KP_6, ceph::KeyCode::Numpad6 },
			{ GLFW_KEY_KP_7, ceph::KeyCode::Numpad7 },
			{ GLFW_KEY_KP_8, ceph::KeyCode::Numpad8 },
			{ GLFW_KEY_KP_9, ceph::KeyCode::Numpad9 },
			{ GLFW_KEY_LEFT_SHIFT, ceph::KeyCode::LShift },
			{ GLFW_KEY_LEFT_CONTROL, ceph::KeyCode::LControl },
			{ GLFW_KEY_LEFT_ALT, ceph::KeyCode::LAlt },
			{ GLFW_KEY_RIGHT_SHIFT, ceph::KeyCode::RShift },
			{ GLFW_KEY_RIGHT_CONTROL, ceph::KeyCode::RControl },
			{ GLFW_KEY_RIGHT_ALT, ceph::KeyCode::RAlt },
			{ GLFW_KEY_MENU, ceph::KeyCode::Menu },
	};

	void OnError(int errorCode, const char* msg) {
		throw std::runtime_error(msg);
	}

	//bool do_scene_transition = false;

	static void key_callback(GLFWwindow* window, int key_index, int scancode, int action, int mods)
	{
		if (action == GLFW_REPEAT) // TODO: Handle this?
			return;

		ceph::KeyCode key = (map_glfw_to_ceph_key.find(key_index) != map_glfw_to_ceph_key.end()) ?
			map_glfw_to_ceph_key.at(key_index) : ceph::KeyCode::Unknown;

		ceph::Game::getInstance().keyEvent.fire( (action == GLFW_PRESS), key, static_cast<ceph::KeyModifiers>(mods) );
	}

	GLFWwindow* CreateGlWindow(ceph::WindowMode mode, ceph::VideoMode vm, const char* title = "cephalopod")
	{
		if (mode == ceph::WindowMode::FullScreen) {
			auto m = glfwGetVideoMode(glfwGetPrimaryMonitor());
			vm.width = (vm.width < 0) ? m->width : vm.width;
			vm.height = (vm.height < 0) ? m->height : vm.height;
			vm.refresh_rate = (vm.refresh_rate < 0) ? m->refreshRate : vm.refresh_rate;
			vm.red_bits = (vm.red_bits < 0) ? m->redBits : vm.red_bits;
			vm.green_bits = (vm.green_bits < 0) ? m->greenBits : vm.green_bits;
			vm.blue_bits = (vm.blue_bits < 0) ? m->blueBits : vm.blue_bits;
		}

		// open a window with GLFW
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		auto monitor = (mode == ceph::WindowMode::FullScreen || mode == ceph::WindowMode::FullScreenWindowed) ?
			glfwGetPrimaryMonitor() : NULL;

		if (mode == ceph::WindowMode::FullScreenWindowed) {
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			vm.width = mode->width;
			vm.height = mode->height;
		} else if (mode == ceph::WindowMode::FullScreen) {
			glfwWindowHint(GLFW_RED_BITS, vm.red_bits);
			glfwWindowHint(GLFW_GREEN_BITS, vm.green_bits);
			glfwWindowHint(GLFW_BLUE_BITS, vm.blue_bits);
			glfwWindowHint(GLFW_REFRESH_RATE, vm.refresh_rate);
		}

		auto window = glfwCreateWindow( vm.width, vm.height, title, monitor, NULL );
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

namespace ceph
{
	class GameImpl
	{
	public:
		GLFWwindow* window_;
		std::unique_ptr<ceph::Graphics> graphics_;
		ceph::Scene* active_scene_;
		ceph::CoordinateMapping coord_mapping_mode_;
		ceph::Vec2<float> log_size_;
	};
}

ceph::Game::Game() 
{
	g_instance_ = this;

	impl_ = std::make_unique<GameImpl>();
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	// initialise GLFW
	glfwSetErrorCallback(OnError);
	if (!glfwInit())
		throw std::runtime_error("glfwInit failed");
}

void ceph::Game::initializeFullscreen(VideoMode vm, const std::string& title)
{
	impl_->window_ = CreateGlWindow(ceph::WindowMode::FullScreen, vm, title.c_str());
	impl_->graphics_ = std::make_unique<ceph::Graphics>(impl_->window_);
}

void ceph::Game::initialize(ceph::WindowMode mode, int wd, int hgt, const std::string& title)
{
	impl_->window_ = CreateGlWindow(mode, VideoMode(wd,hgt), title.c_str());
	impl_->graphics_ = std::make_unique<ceph::Graphics>(impl_->window_);
}

void ceph::Game::setLogicalCoordinates(ceph::CoordinateMapping mapping, const ceph::Vec2<float>& log_size, ceph::CoordinateSystem system)
{
	impl_->graphics_->setCoordinateSystem(system, mapping, log_size);
}

/*
void TestPanTransition(ceph::DrawingContext& dc, float elapsed)
{
	if (do_scene_transition)
	{
		ceph::Mat3x3 translate = ceph::Mat3x3().translate(-400.0f * elapsed, 0);
		dc.transformation *= translate;
	}
}
*/

bool ceph::Game::isInSceneTransition() const
{
	return transition_.get();
}

void ceph::Game::clearTransition()
{
	transition_ = nullptr;
}

ceph::SceneTransition& ceph::Game::getSceneTransition()
{
	return *transition_;
}

void ceph::Game::setScene(const std::string& scene_name)
{
	clearTransition();
	switchScenes(scene_name);
}

void ceph::Game::switchScenes(const std::string& scene_name)
{
	auto old_scene = impl_->active_scene_;
	auto new_scene = impl_->active_scene_ = scenes_[scene_name].get();
	if (transition_)
		transition_->setScenes(*old_scene, *new_scene);
}

void ceph::Game::run(const std::string& scene_name) {
	ceph::Clock clock;

	setScene(scene_name);
	impl_->active_scene_->onBecomeActive();

	while (!glfwWindowShouldClose(impl_->window_)) {
		auto old_scene = impl_->active_scene_;
		glfwPollEvents();
		auto elapsed = clock.restart();

		if (! isInSceneTransition()) {
			DrawingContext dc(*impl_->graphics_);
			auto& scene = getActiveScene();
			scene.update(elapsed);
			impl_->graphics_->BeginFrame();
			scene.draw(dc);
			impl_->graphics_->EndFrame();
			scene.endGameLoopIteration();
		} else {
			DrawingContext dc(*impl_->graphics_);
			auto& transition = getSceneTransition();
			transition.update(elapsed);
			impl_->graphics_->BeginFrame();
			transition.draw(dc);
			impl_->graphics_->EndFrame();
			transition.endGameLoopIteration();
			if (transition.isComplete())
				clearTransition();
		}

		auto new_scene = impl_->active_scene_;
		if (old_scene != new_scene) {
			if (old_scene)
				old_scene->onBecomeInactive();
			if (new_scene) 
				new_scene->onBecomeActive();
		}
	}
}

ceph::Rect<float> ceph::Game::getLogicalRect() const
{
	auto view_mat = impl_->graphics_->getViewMatrix();
	return view_mat.getInverse().value().apply(ceph::Rect<float>(-1, -1, 2, 2));
}

ceph::Vec2<float> ceph::Game::getLogicalSize() const
{
	return impl_->log_size_;
}

void ceph::Game::quit()
{
	glfwSetWindowShouldClose(impl_->window_, 1);
}

ceph::CoordinateMapping ceph::Game::getCoordinateMapping() const
{
	return impl_->coord_mapping_mode_;
}


ceph::Rect<int> ceph::Game::getScreenRect() const
{
	int wd, hgt;
	glfwGetWindowSize(impl_->window_, &wd, &hgt);
	return ceph::Rect<int>(0, 0, wd, hgt);
}

ceph::Scene& ceph::Game::getActiveScene() const
{
	return *impl_->active_scene_;
}

std::unique_ptr<ceph::Game> ceph::Game::createInstance()
{
	if (g_instance_ != nullptr)
		throw std::runtime_error("only one intsance of ceph::Game is permitted.");
	return std::unique_ptr<Game>(new ceph::Game()); // can't use make_unique with a private constructor
}

std::list<ceph::VideoMode> ceph::Game::getVideoModes() const
{
	int n;
	auto ary = glfwGetVideoModes(glfwGetPrimaryMonitor(), &n);
	if (!ary)
		return std::list<ceph::VideoMode>();
	std::list<ceph::VideoMode> modes;
	for (int i = 0; i < n; i++)
		modes.push_back(
			ceph::VideoMode(
				ary[i].width,
				ary[i].height,
				ary[i].refreshRate,
				ary[i].redBits,
				ary[i].greenBits,
				ary[i].blueBits
			)
		);
	return modes;
}

ceph::Game& ceph::Game::getInstance()
{
	if (!g_instance_)
		throw std::runtime_error("No instance of the cephalopod game singleton");
	return *g_instance_;
}



