#pragma once
#include <memory>
#include <list>
#include <string>
#include <unordered_map>
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
		std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
		std::unique_ptr<SceneTransition> transition_;
		std::unique_ptr<ceph::GameImpl> impl_;

		Game();
		void switchScenes(const std::string& new_scene);

	public:
		
		virtual ~Game() = default;
		void initialize(WindowMode mode, int wd = 0, int hgt = 0, const std::string& title = "");
		void initializeFullscreen(VideoMode vm = VideoMode(), const std::string& title = "");
		std::list<VideoMode> getVideoModes() const;
		void setLogicalCoordinates(CoordinateMapping mapping, const Vec2<float>& logSize, 
			CoordinateSystem system = CoordinateSystem::UpperLeftOriginDescendingY);
		void quit();
		Rect<float> getLogicalRect() const;
		Vec2<float> getLogicalSize() const;
		Rect<int> getScreenRect() const;
		CoordinateMapping getCoordinateMapping() const;
		Scene& getActiveScene() const;
		bool isInSceneTransition() const;
		void clearTransition();
		SceneTransition& getSceneTransition();

		void run(const std::string& initial_scene);

		Signal<bool, KeyCode, KeyModifiers> keyEvent;
		static Game& getInstance();
		static std::unique_ptr<ceph::Game> ceph::Game::createInstance();

		template<typename T, typename... Args>
		void registerScene(const std::string& s, Args&&... args) {
			scenes_.emplace(std::make_pair(s, std::make_unique<T>(args...)));
		}

		template<typename T, typename... Args>
		void setScene(const std::string& scene_name, Args&&... args) {
			transition_ = std::make_unique<T>(args...);
			switchScenes(scene_name);
		}

		void setScene(const std::string& scene_name);
	};
}