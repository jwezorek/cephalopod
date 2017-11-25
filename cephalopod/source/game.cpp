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
#include "gameimpl.hpp"

static std::vector<sf::RectangleShape*> debug_rects;

void ceph::GameImpl::handleInput(const sf::Event& evt)
{
	switch (evt.type) {
		case sf::Event::KeyPressed:
			keyEvent.fire(true, static_cast<ceph::KeyCode>(evt.key.code), getModifiers(evt.key));
			break;
		case sf::Event::KeyReleased:
			keyEvent.fire(false, static_cast<ceph::KeyCode>(evt.key.code), getModifiers(evt.key));
			break;
	}
}

unsigned char ceph::GameImpl::getModifiers(const sf::Event::KeyEvent& ke) {
	//TODO
	return 0;
}

sf::Transform ceph::GameImpl::getCoordinateSystemMatrix(ceph::CoordinateSystem system, const ceph::Size<float>& log_sz)
{
	if (system == ceph::CoordinateSystem::UpperLeftOriginDescendingY)
		return sf::Transform::Identity;

	sf::Transform inverted_y = sf::Transform().scale(1.0f, -1.0f);
	sf::Vector2f orig_pt = (system == ceph::CoordinateSystem::LowerLeftOriginAscendingY) ?
		sf::Vector2f(0.0f, -log_sz.hgt) :
		sf::Vector2f(static_cast<float>(log_sz.wd) / 2.0f, -static_cast<float>(log_sz.hgt) / 2.0f);

	return inverted_y.translate(orig_pt);
}

ceph::Size<float> ceph::GameImpl::getLogSizeFromMapping(ceph::CoordinateMapping mapping_mode, const ceph::Size<float>& log_size, const ceph::Size<int>& scr_size)
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

std::tuple<sf::Transform, std::vector<sf::RectangleShape>> ceph::GameImpl::getCoordinateMappingInfo(
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

ceph::GameImpl::GameImpl() {
	ceph::GameImpl::instance_ = this;
}

ceph::GameImpl* ceph::GameImpl::getInstance()
{
	return instance_;
}

void ceph::GameImpl::initialize(ceph::ScreenMode mode, int wd, int hgt, const std::string& title)
{
	sf::VideoMode video_mode;
	unsigned int style;
	switch (mode) {
		case ceph::ScreenMode::FullScreenExclusive:
			video_mode = sf::VideoMode::getFullscreenModes()[0];
			wd = video_mode.width;
			hgt = video_mode.height;
			style = sf::Style::Fullscreen;
			break;
		case ceph::ScreenMode::FullScreenWindowed:
			video_mode = sf::VideoMode::getDesktopMode();
			wd = video_mode.width;
			hgt = video_mode.height;
			style = sf::Style::None;
			break;
		case ceph::ScreenMode::WindowedWithTitleBar:
			video_mode = sf::VideoMode(wd, hgt);
			style = sf::Style::Titlebar | sf::Style::Close;
			break;
	}

	window_ = std::make_unique<sf::RenderWindow>( video_mode, title, style );
	setLogicalCoordinates(
		ceph::CoordinateMapping::StretchToFit, 
		ceph::Size<float>(static_cast<float>(wd), static_cast<float>(hgt)),
		ceph::CoordinateSystem::UpperLeftOriginDescendingY
	);
}

void ceph::GameImpl::setLogicalCoordinates(ceph::CoordinateMapping mapping, const ceph::Size<float>& log_size, ceph::CoordinateSystem system)
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

void ceph::GameImpl::drawBlackBars()
{
	if (black_bars_.get()) {
		for( auto& bar: *black_bars_)
			window_->draw(bar);
	}
}

sf::Color CephToSfColor(const ceph::ColorRGB& cc)
{
	return sf::Color(cc.r, cc.g, cc.b, 255);
}

void ceph::GameImpl::run(const std::shared_ptr<ceph::Scene>& startingScene) {
	active_scene_ = startingScene;

	sf::Event event;
	sf::Clock clock;
	
	while (window_->isOpen()) {
		while (window_->pollEvent(event)) {
			if (event.type == sf::Event::EventType::Closed)
				window_->close();
			handleInput(event);
		}

		auto elapsed = clock.getElapsedTime().asSeconds();
		active_scene_->updateActionsEvent.fire(elapsed);
		active_scene_->updateEvent.fire(elapsed);
		clock.restart();

		window_->clear( CephToSfColor(active_scene_->getBackgroundColor()) );

		active_scene_->draw( ceph::DrawingContext( *window_, coord_transform_));

		for (auto rs : debug_rects)
			window_->draw(*rs);

		if (coord_mapping_mode_ == ceph::CoordinateMapping::UseBlackBars)
			drawBlackBars();

		window_->display();
	}
}

ceph::Rect<float> ceph::GameImpl::getLogicalRect()
{
	return ceph::SfmlRectToCoyRect( 
		coord_system_.getInverse().transformRect(
			sf::Rect<float>(0, 0, log_size_.wd, log_size_.hgt)
		) 
	);
}

ceph::Size<int> ceph::GameImpl::getScreenSize()
{
	auto sz = window_->getSize();
	return { static_cast<int>(sz.x), static_cast<int>(sz.y) };
}

ceph::Size<float> ceph::GameImpl::getLogicalSize()
{
	return log_size_;
}

void ceph::GameImpl::quit()
{
	window_->close();
}

ceph::CoordinateMapping ceph::GameImpl::getCoordinateMapping() const
{
	return coord_mapping_mode_;
}

void ceph::GameImpl::addDebugRect(const Rect<float>& rect)
{
	auto rs = new sf::RectangleShape(sf::Vector2f(rect.wd, rect.hgt));
	rs->setPosition(rect.x, rect.y);
	rs->setFillColor(sf::Color::Transparent);
	rs->setOutlineColor(sf::Color::Green);
	rs->setOutlineThickness( 1 );
	debug_rects.push_back( rs );
}

sf::Transform ceph::GameImpl::getCoordTransform() const
{
	return coord_transform_;
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



