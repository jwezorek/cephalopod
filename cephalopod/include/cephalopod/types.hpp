#pragma once

#include <memory>

namespace ceph {
	
	enum class CoordinateMapping
	{
		StretchToFit,
		UseBlackBars,
		PreserveWidth,
		PreserveHeight
	};

	enum class CoordinateSystem
	{
		UpperLeftOriginDescendingY,
		CenterOriginAscendingY,
		LowerLeftOriginAscendingY
	};

	template<typename T>
	struct Vec2D {
		T x, y;
		Vec2D(T xx = 0, T yy = 0) :
			x(xx), y(yy)
		{
		}
	};

	template<typename T>
	struct Point {
		T x, y;
		Point(T xx = 0, T yy = 0) :
			x(xx), y(yy)
		{
		}
	};

	template<typename T>
	struct Size {
		T wd;
		T hgt;

		Size(T w = 0, T h = 0) : wd(w), hgt(h)
		{}
	};

	template<typename T>
	struct Rect {
		T x;
		T y;
		T wd;
		T hgt;

		Rect(const Point<T> loc, const Size<T> sz) :
			x(loc.x), y(loc.y), wd(sz.wd), hgt(sz.hgt) {
		}

		Rect(T xx = 0, T yy = 0, T w = 0, T h = 0) :
			x(xx), y(yy), wd(w), hgt(h) {
		}

		Point<T> getLocation()
		{
			return Point<T>(x, y);
		}

		Size<T> getSize()
		{
			return Size<T>(wd, hgt);
		}
	};
}