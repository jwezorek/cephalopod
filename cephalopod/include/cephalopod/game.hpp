#pragma once
#include <memory>
#include <list>
#include <string>
#include "scene.hpp"
#include "types.hpp"
#include "signals.hpp"

namespace ceph
{
	enum class WindowMode
	{
		FullScreen,
		FullScreenWindowed,
		WindowedWithTitleBar
	};

	struct VideoMode
	{
		int width;
		int height;
		int refresh_rate;
		int red_bits;
		int green_bits;
		int blue_bits;

		VideoMode(int wd=-1, int hgt=-1, int refresh = -1, int r = -1, int g = -1, int b = -1) :
			width(wd), height(hgt), refresh_rate(-1), red_bits(r), green_bits(g), blue_bits(b)
		{}
	};

	class SceneTransition;

	class Game : public Slot<Game> 
	{
	public:
		virtual ~Game() = default;
		virtual void initialize(WindowMode mode, int wd = 0, int hgt = 0, const std::string& title = "") = 0;
		virtual void initializeFullscreen(VideoMode vm = VideoMode(), const std::string& title = "") = 0;
		virtual std::list<VideoMode> getVideoModes() const = 0;
		virtual void setLogicalCoordinates(CoordinateMapping mapping, const Vec2<float>& logSize, 
			CoordinateSystem system = CoordinateSystem::UpperLeftOriginDescendingY) = 0;
		virtual void run() = 0;
		virtual void quit() = 0;
		virtual Rect<float> getLogicalRect() const = 0;
		virtual Vec2<float> getLogicalSize() const = 0;
		virtual Rect<int> getScreenRect() const = 0;
		virtual CoordinateMapping getCoordinateMapping() const = 0;
		virtual std::shared_ptr<Scene> getActiveScene() const = 0;
		virtual bool isInSceneTransition() const = 0;
		virtual void clearTransition() = 0;
		virtual SceneTransition& getSceneTransition() = 0;
		virtual void setScene(const std::shared_ptr<Scene>& scene, const std::shared_ptr<SceneTransition> transition = nullptr) = 0;

		Signal<bool, KeyCode, unsigned char> keyEvent;
		static Game& getInstance();
		static std::unique_ptr<ceph::Game> ceph::Game::createInstance();
	};
}