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

		//void handleInput(const sf::Event& evt);
		//static unsigned char getModifiers(const sf::Event::KeyEvent& ke);
		//static sf::Transform getCoordinateSystemMatrix(ceph::CoordinateSystem system, const ceph::Size<float>& log_sz);

		//static ceph::Size<float> getLogSizeFromMapping(ceph::CoordinateMapping mapping_mode, const ceph::Size<float>& log_size, const ceph::Size<int>& scr_size);

		//static std::tuple<sf::Transform, std::vector<sf::RectangleShape>> getCoordinateMappingInfo(
		//	ceph::CoordinateMapping mapping_mode,
		//	const ceph::Size<float>& log_sz, const ceph::Size<int>& scr_sz);

	public:

		GameImpl();

		static GameImpl* getInstance();
		void initialize(ceph::ScreenMode mode, int wd, int hgt, const std::string& title) override;
		void setLogicalCoordinates(CoordinateMapping mapping, const Vec2<float>& log_size, CoordinateSystem system);
		void run(const std::shared_ptr<ceph::Scene>& startingScene) override;
		ceph::Rect<float> getLogicalRect() const override;
		ceph::Vec2<int> getScreenSize() const override;
		ceph::Vec2<float> getLogicalSize() const override;
		void quit() override;
		ceph::CoordinateMapping getCoordinateMapping() const override;
		Rect<float> getScreenRect() const override;
		std::shared_ptr<Scene> getActiveScene() const override;
	};
};