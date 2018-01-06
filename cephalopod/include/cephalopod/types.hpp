#pragma once

#include <cstddef>
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

	struct ColorRGB
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;

		ColorRGB(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0) : r(r), g(g), b(b) {}
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
	using Vec2D = Point<T>;

	template<typename T>
	Vec2D<T> operator*(T k, Vec2D<T> v)
	{
		return Vec2D<T>(k * v.x, k * v.y);
	}

	template<typename T>
	Vec2D<T> operator+(Vec2D<T> v1, Vec2D<T> v2)
	{
		return Vec2D<T>(v1.x + v2.x, v1.y + v2.y);
	}

	template<typename T>
	Vec2D<T> operator-(Vec2D<T> v1, Vec2D<T> v2)
	{
		return Vec2D<T>(v1.x - v2.x, v1.y - v2.y);
	}

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

		Point<T> getLocation() const
		{
			return Point<T>(x, y);
		}

		Size<T> getSize() const
		{
			return Size<T>(wd, hgt);
		}

		T x2() const
		{
			return x + wd;
		}

		T y2() const
		{
			return y + hgt;
		}

		T area() const
		{
			return wd*hgt;
		}

		Rect<T> unionOf(const Rect<T>& r)
		{
			T new_x = std::min(x, r.x);
			T new_y = std::min(y, r.y);
			T new_x2 = std::max(x2(), r.x2());
			T new_y2 = std::max(y2(), r.y2());
			return Rect<T>(new_x, new_y, new_x2 - new_x, new_y2 - new_y);
		}

		void unionWith(const Rect<T>& r)
		{
			*this = unionOf(r);
		}

		bool intersects(const Rect<T>& r)
		{
			return (x < r.x2() && x2() > r.x && y < r.y2() && y2() > r.y);
		}

		void inflate(T horz, T vert)
		{
			x -= horz;
			y -= vert;
			wd += 2 * horz;
			hgt += 2 * vert;
		}

		bool contains(T px, T py)
		{
			float rx = x, ry = y;
			return  (rx <= px) && (px <= rx + wd) && (ry <= py) && (py <= ry + hgt);
		}

		bool contains(const Point<T>& p)
		{
			return contains(p.x, p.y);
		}
	};
}