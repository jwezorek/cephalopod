#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
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
#include "drawingcontext.hpp"
#include "util.hpp"

namespace
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

		void handleInput(const sf::Event& evt)
		{
			static std::unordered_map<sf::Event::EventType, std::function<void(const std::shared_ptr<ceph::Scene> s, const sf::Event& e)>> handlers = {
				{ sf::Event::KeyPressed,
				[=](const std::shared_ptr<ceph::Scene> s, const sf::Event& e) {
				s->handleKeyEvent(true, static_cast<ceph::KeyCode>(e.key.code), getModifiers(e.key));
			},
				},
				{ sf::Event::KeyReleased,
				[=](const std::shared_ptr<ceph::Scene> s, const sf::Event& e) {
				s->handleKeyEvent(false, static_cast<ceph::KeyCode>(e.key.code), getModifiers(e.key));
			}
				}
			};
			if (handlers.find(evt.type) != handlers.end()) {
				const auto& handler = handlers[evt.type];
				handler(active_scene_, evt);
			}
		}

		void update(float elapsed) const
		{
			active_scene_->update(elapsed);
		}

		static unsigned char getModifiers(const sf::Event::KeyEvent& ke) {
			//TODO
			return 0;
		}

		static sf::Transform getCoordinateSystemMatrix(ceph::CoordinateSystem system, const ceph::Size<float>& log_sz)
		{
			if (system == ceph::CoordinateSystem::UpperLeftOriginDescendingY)
				return sf::Transform::Identity;

			sf::Transform inverted_y = sf::Transform().scale(1.0f, -1.0f);
			sf::Vector2f orig_pt = (system == ceph::CoordinateSystem::LowerLeftOriginAscendingY) ?
				sf::Vector2f(0.0f, -log_sz.hgt) :
				sf::Vector2f(static_cast<float>(log_sz.wd) / 2.0f, -static_cast<float>(log_sz.hgt) / 2.0f);

			return inverted_y.translate(orig_pt);
		}

		static ceph::Size<float> getLogSizeFromMapping(ceph::CoordinateMapping mapping_mode, const ceph::Size<float>& log_size, const ceph::Size<int>& scr_size)
		{
			float aspect_ratio = static_cast<float>(scr_size.wd) / static_cast<float>(scr_size.hgt);
			switch (mapping_mode) {
				case ceph::CoordinateMapping::StretchToFit:
				case ceph::CoordinateMapping::UseBlackBars:
					return log_size;
				case ceph::CoordinateMapping::PreserveHeight:
					return ceph::Size<float>(
						log_size.hgt * aspect_ratio,
						log_size.hgt
					);
				case ceph::CoordinateMapping::PreserveWidth:
					return ceph::Size<float>(
						log_size.wd,
						log_size.hgt * (1.0f / aspect_ratio)
					);
			}
			return  ceph::Size<float>(0, 0);
		}

		static auto getCoordinateMappingInfo(
			ceph::CoordinateMapping mapping_mode, 
			const ceph::Size<float>& log_sz, 
			const ceph::Size<int>& scr_sz)
		{
			sf::Transform mapping;
			std::vector<ceph::Rect<float>> black_bar_rects;
			std::vector<sf::RectangleShape> bars;
			switch (mapping_mode) {
				case ceph::CoordinateMapping::StretchToFit:
					mapping = getStretchToFitMatrix(log_sz, scr_sz);
					break;
				case ceph::CoordinateMapping::UseBlackBars:
					mapping = getUseBlackBarsMatrix(log_sz, scr_sz, &black_bar_rects);
					bars.resize(2);
					std::transform(black_bar_rects.cbegin(), black_bar_rects.cend(), bars.begin(),
						[](const ceph::Rect<float>& r) -> sf::RectangleShape {
							sf::RectangleShape rs(sf::Vector2f(r.wd, r.hgt));
							rs.setPosition(r.x, r.y);
							rs.setFillColor(sf::Color(0, 0, 0));
							return rs;
						}
					);
					break;
				case ceph::CoordinateMapping::PreserveHeight:
				case ceph::CoordinateMapping::PreserveWidth: 
					float scale = static_cast<float>(scr_sz.hgt) / log_sz.hgt;
					mapping = sf::Transform().scale(scale, scale);
					break;
			}
			return std::tuple<sf::Transform, std::vector<sf::RectangleShape>>( mapping, bars );
		}

	public:

		GameImpl() {
		}

		void initialize(ceph::ScreenMode mode, int wd, int hgt, const std::string& title) override
		{
			sf::VideoMode video_mode;
			unsigned int style;
			switch (mode) {
				case ceph::ScreenMode::FullScreenExclusive:
					video_mode = sf::VideoMode::getFullscreenModes()[0];
					style = sf::Style::Fullscreen;
					break;
				case ceph::ScreenMode::FullScreenWindowed:
					video_mode = sf::VideoMode::getDesktopMode();
					style = sf::Style::None;
					break;
				case ceph::ScreenMode::WindowedWithTitleBar:
					video_mode = sf::VideoMode(wd, hgt);
					style = sf::Style::Titlebar;
					break;
			}

			window_ = std::make_unique<sf::RenderWindow>( video_mode, title, style );
		}

		void setLogicalCoordinates(ceph::CoordinateMapping mapping, const ceph::Size<float>& log_size, ceph::CoordinateSystem system) override
		{
			auto scr_sz = getScreenSize();
			log_size_ = getLogSizeFromMapping(mapping, log_size, scr_sz);
			auto mapping_info = getCoordinateMappingInfo(mapping, log_size_, scr_sz);

			coord_mapping_mode_ = mapping;
			coord_mapping_ = std::get<0>(mapping_info);
			auto bars = std::get<1>(mapping_info);
			black_bars_ = (!bars.empty()) ? 
				std::make_unique<std::vector<sf::RectangleShape>>(bars) : 
				nullptr;

			coord_transform_ = coord_mapping_.combine(
				coord_system_ = getCoordinateSystemMatrix(system, log_size_)
			);
		}

		void drawBlackBars()
		{
			if (black_bars_.get()) {
				for( auto& bar: *black_bars_)
					window_->draw(bar);
			}
		}

		void run(const std::shared_ptr<ceph::Scene>& startingScene) override {
			active_scene_ = startingScene;

			sf::Event event;
			sf::Clock clock;

			while (window_->isOpen()) {
				while (window_->pollEvent(event)) {
					if (event.type == sf::Event::EventType::Closed)
						window_->close();
					handleInput(event);
				}
				update( clock.getElapsedTime().asSeconds() );

				clock.restart();
				window_->clear();

				active_scene_->draw( ceph::DrawingContext( *window_, coord_transform_));

				if (coord_mapping_mode_ == ceph::CoordinateMapping::UseBlackBars)
					drawBlackBars();

				window_->display();
			}
		}

		ceph::Rect<float> getLogicalRect() override
		{
			return ceph::SfmlRectToCoyRect( 
				coord_system_.getInverse().transformRect(
					sf::Rect<float>(0, 0, log_size_.wd, log_size_.hgt)
				) 
			);
		}

		ceph::Size<int> getScreenSize() override
		{
			auto sz = window_->getSize();
			return { static_cast<int>(sz.x), static_cast<int>(sz.y) };
		}

		ceph::Size<float> getLogicalSize() override
		{
			return log_size_;
		}

		void quit() override
		{
			window_->close();
		}

		ceph::CoordinateMapping getCoordinateMapping() const override
		{
			return coord_mapping_mode_;
		}

	};


	GameImpl g_game;
}


ceph::Game& ceph::Game::getInstance()
{
	return g_game;
}

/*
KeyPressed,             ///< A key was pressed (data in event.key)
KeyReleased,            ///< A key was released (data in event.key)
MouseWheelMoved,        ///< The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
MouseWheelScrolled,     ///< The mouse wheel was scrolled (data in event.mouseWheelScroll)
MouseButtonPressed,     ///< A mouse button was pressed (data in event.mouseButton)
MouseButtonReleased,    ///< A mouse button was released (data in event.mouseButton)
MouseMoved,
*/
