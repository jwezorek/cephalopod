#pragma once

#include <memory>
#include <unordered_map>
#include "graphics.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/scene.hpp"

struct GLFWwindow;

namespace ceph
{
	class GameImpl : public ceph::Game
	{
	private:
		GLFWwindow* window_;
		std::unique_ptr<ceph::Graphics> graphics_;

		std::shared_ptr<ceph::Scene> active_scene_;
		ceph::CoordinateMapping coord_mapping_mode_;
		ceph::Vec2<float> log_size_;
		static GameImpl* instance_;

	public:

		GameImpl();

		static GameImpl* getInstance();
		void initialize(ceph::WindowMode mode, int wd, int hgt, const std::string& title) override;
		void setLogicalCoordinates(CoordinateMapping mapping, const Vec2<float>& log_size, CoordinateSystem system);
		void run(const std::shared_ptr<ceph::Scene>& startingScene) override;
		ceph::Rect<float> getLogicalRect() const override;
		ceph::Vec2<float> getLogicalSize() const override;
		void quit() override;
		ceph::CoordinateMapping getCoordinateMapping() const override;
		Rect<int> getScreenRect() const override;
		std::shared_ptr<Scene> getActiveScene() const override;
		void initializeFullscreen(VideoMode vm, const std::string& title) override;
		std::list<VideoMode> getVideoModes() const override;
	};
};