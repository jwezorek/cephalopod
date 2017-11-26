#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/scene.hpp"

namespace ceph
{
	class GameImpl : public ceph::Game
	{
	private:
		std::unique_ptr<sf::RenderWindow> window_;
		std::shared_ptr<ceph::Scene> active_scene_;
		std::unique_ptr<std::vector<sf::RectangleShape>> black_bars_;
		ceph::CoordinateMapping coord_mapping_mode_;
		ceph::Size<float> log_size_;
		sf::Transform coord_mapping_;
		sf::Transform coord_system_;
		sf::Transform coord_transform_;
		static GameImpl* instance_;

		void handleInput(const sf::Event& evt);
		static unsigned char getModifiers(const sf::Event::KeyEvent& ke);
		static sf::Transform getCoordinateSystemMatrix(ceph::CoordinateSystem system, const ceph::Size<float>& log_sz);

		static ceph::Size<float> getLogSizeFromMapping(ceph::CoordinateMapping mapping_mode, const ceph::Size<float>& log_size, const ceph::Size<int>& scr_size);

		static std::tuple<sf::Transform, std::vector<sf::RectangleShape>> getCoordinateMappingInfo(
			ceph::CoordinateMapping mapping_mode,
			const ceph::Size<float>& log_sz, const ceph::Size<int>& scr_sz);

	public:

		GameImpl();
		static GameImpl* getInstance();
		void initialize(ceph::ScreenMode mode, int wd, int hgt, const std::string& title) override;
		void setLogicalCoordinates(ceph::CoordinateMapping mapping, const ceph::Size<float>& log_size, ceph::CoordinateSystem system) override;
		void drawBlackBars();
		void run(const std::shared_ptr<ceph::Scene>& startingScene) override;
		ceph::Rect<float> getLogicalRect() const override;
		ceph::Size<int> getScreenSize() const override;
		ceph::Size<float> getLogicalSize() const override;
		void quit() override;
		ceph::CoordinateMapping getCoordinateMapping() const override;
		void addDebugRect(const Rect<float>& rect) override;
		sf::Transform getCoordTransform() const;
		Rect<float> getScreenRect() const override;
		Rect<float> convertToScreenCoords(const Rect<float>& rect) const override;
		Rect<float> convertFromScreenCoords(const Rect<float>& rect) const override;
		Point<float> convertToScreenCoords(const Point<float>& rect) const override;
		Point<float> convertFromScreenCoords(const Point<float>& rect)  const override;
	};
};