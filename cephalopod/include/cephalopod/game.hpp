#pragma once
#include <memory>
#include <string>
#include "scene.hpp"
#include "types.hpp"
#include "signals.hpp"

namespace ceph
{
	enum class ScreenMode
	{
		FullScreenExclusive,
		FullScreenWindowed,
		WindowedWithTitleBar
	};

	class Game : public Slot<Game> 
	{
	public:
		virtual ~Game() = default;
		virtual void initialize(ScreenMode mode, int wd = 0, int hgt = 0, const std::string& title = "") = 0;
		virtual void setLogicalCoordinates(CoordinateMapping mapping, const Vec2<float>& logSize, 
			CoordinateSystem system = CoordinateSystem::UpperLeftOriginDescendingY) = 0;
		virtual void run(const std::shared_ptr<Scene>& startingScene ) = 0;
		virtual void quit() = 0;
		virtual Vec2<int> getScreenSize() const = 0;
		virtual Rect<float> getLogicalRect() const = 0;
		virtual Vec2<float> getLogicalSize() const = 0;
		virtual Rect<float> getScreenRect() const = 0;

		virtual Rect<float> convertToScreenCoords(const Rect<float>& rect) const = 0;
		virtual Rect<float> convertFromScreenCoords(const Rect<float>& rect) const = 0;
		virtual Vec2<float> convertToScreenCoords(const Vec2<float>& rect) const = 0;
		virtual Vec2<float> convertFromScreenCoords(const Vec2<float>& rect) const = 0;

		virtual CoordinateMapping getCoordinateMapping() const = 0;
		virtual std::shared_ptr<Scene> getActiveScene() const = 0;

		Signal<bool, KeyCode, unsigned char> keyEvent;

		static Game& getInstance();
		static std::unique_ptr<ceph::Game> ceph::Game::createInstance();
	};
}