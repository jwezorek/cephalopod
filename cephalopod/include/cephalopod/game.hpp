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
	class GameImpl;

	class Game : public Slot<Game> 
	{
	private:
		std::unique_ptr<ceph::GameImpl> impl_;
		Game();
	public:
		
		virtual ~Game() = default;
		void initialize(WindowMode mode, int wd = 0, int hgt = 0, const std::string& title = "");
		void initializeFullscreen(VideoMode vm = VideoMode(), const std::string& title = "");
		std::list<VideoMode> getVideoModes() const;
		void setLogicalCoordinates(CoordinateMapping mapping, const Vec2<float>& logSize, 
			CoordinateSystem system = CoordinateSystem::UpperLeftOriginDescendingY);
		void run();
		void quit();
		Rect<float> getLogicalRect() const;
		Vec2<float> getLogicalSize() const;
		Rect<int> getScreenRect() const;
		CoordinateMapping getCoordinateMapping() const;
		std::shared_ptr<Scene> getActiveScene() const;
		bool isInSceneTransition() const;
		void clearTransition();
		SceneTransition& getSceneTransition();
		void setScene(const std::shared_ptr<Scene>& scene, const std::shared_ptr<SceneTransition> transition = nullptr);

		Signal<bool, KeyCode, unsigned char> keyEvent;
		static Game& getInstance();
		static std::unique_ptr<ceph::Game> ceph::Game::createInstance();
	};
}