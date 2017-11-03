#pragma once
#include <memory>
#include "scene.hpp"
#include "types.hpp"

namespace ceph
{
	enum class ScreenMode
	{
		FullScreenExclusive,
		FullScreenWindowed,
		WindowedWithTitleBar
	};

	class Game
	{
	public:
		virtual ~Game() = default;
		virtual void initialize(ScreenMode mode, int wd = 0, int hgt = 0, const std::string& title = "") = 0;
		virtual void setLogicalCoordinates(CoordinateMapping mapping, const Size<float>& logSize, 
			CoordinateSystem system = CoordinateSystem::UpperLeftOriginDescendingY) = 0;
		virtual void run(const std::shared_ptr<Scene>& startingScene ) = 0;
		virtual void quit() = 0;
		virtual Size<int> getScreenSize() = 0;
		virtual Rect<float> getLogicalRect() = 0;
		virtual Size<float> getLogicalSize() = 0;
		virtual CoordinateMapping getCoordinateMapping() const = 0;

		static Game& getInstance();
	};
}