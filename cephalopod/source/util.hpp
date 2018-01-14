#pragma once
#include <memory>
#include <string>
#include <array>
#include "SFML/Graphics.hpp"
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/types.hpp"

namespace ceph {
	void outputDebugString(const std::string& str);

	float radiansToDegrees(float radians);
	float degreesToRadians(float degrees);
	float lerp(float start, float end, float pcnt);
	float distance(float x1, float y1, float x2, float y2);
	float magnitude(float x, float y);
	float magnitude(Vec2D<float> vec);
	Vec2D<float> lerpPtInRect(const Vec2D<float>& pt, const Rect<float>& r);

	template<typename T>
	Rect<T> SfmlRectToCoyRect(sf::Rect<T>& rect)
	{
		return Rect<T>(rect.left, rect.top, rect.width, rect.height);
	}

	template<typename T>
	Vec2D<T> SfPtToCephPt(sf::Vector2<T> pt)
	{
		return ceph::Vec2D<T>(pt.x, pt.y);
	}

	template<typename T>
	sf::Vector2<T> CephPtToSfPt( Vec2D<T> pt)
	{
		return sf::Vector2<T>(pt.x, pt.y);
	}

	template<typename T, typename U>
	sf::Transform getStretchToFitMatrix(const ceph::Size<T>& logSize, const ceph::Size<U>& screenSize)
	{
		return sf::Transform().scale(
			static_cast<float>(screenSize.wd) / static_cast<float>(logSize.wd),
			static_cast<float>(screenSize.hgt) / static_cast<float>(logSize.hgt)
		);
	}

	template<typename T, typename U>
	sf::Transform getCenterBkgdMatrix(const ceph::Size<T>& logSize, const ceph::Size<U>& screenSize, bool useHeight)
	{
		float logAspectRatio = static_cast<float>(logSize.hgt) / static_cast<float>(logSize.wd);
		float scrAspectRatio = static_cast<float>(screenSize.hgt) / static_cast<float>(screenSize.wd);
		float scale, x, y, wd, hgt;

		if (useHeight) {
			// center horz (pillar box or reverse pillar box)
			scale = static_cast<float>(screenSize.hgt) / static_cast<float>(logSize.hgt);
			wd = scale * static_cast<float>(logSize.wd);
			hgt = static_cast<float>(screenSize.hgt);
			x = (screenSize.wd - wd) / 2.0f;
			y = 0.0f;
		}
		else {
			// center vert (letter box or reverse letter box)
			scale = static_cast<float>(screenSize.wd) / static_cast<float>(logSize.wd);
			hgt = scale * static_cast<float>(logSize.hgt);
			wd = static_cast<float>(screenSize.wd);
			x = 0.0f;
			y = (screenSize.hgt - hgt) / 2.0f;
		}

		return sf::Transform().scale(
			scale, scale
		).translate(
			x / scale, y / scale
		);
	}

	template<typename T, typename U>
	sf::Transform getUseBlackBarsMatrix(const ceph::Size<T>& logSize, const ceph::Size<U>& screenSize, std::vector<ceph::Rect<float>>* bars = nullptr)
	{
		float logAspectRatio = static_cast<float>(logSize.hgt) / static_cast<float>(logSize.wd);
		float scrAspectRatio = static_cast<float>(screenSize.hgt) / static_cast<float>(screenSize.wd);
		float scale, x, y, wd, hgt;

		if (logAspectRatio < scrAspectRatio) {
			// letter box
			scale = static_cast<float>(screenSize.wd) / static_cast<float>(logSize.wd);
			wd = static_cast<float>(screenSize.wd);
			hgt = logAspectRatio * wd;
			x = 0.0f;
			y = (screenSize.hgt - hgt) / 2.0f;

			if (bars) {
				bars->push_back(ceph::Rect<float>(0.0f, 0.0f, wd, y));
				bars->push_back(ceph::Rect<float>(0.0f, static_cast<float>(screenSize.hgt) - y, wd, y ));
			}
		}
		else {
			// pillar box
			scale = static_cast<float>(screenSize.hgt) / static_cast<float>(logSize.hgt);
			wd = (1.0f / logAspectRatio) * screenSize.hgt;
			hgt = static_cast<float>(screenSize.hgt);
			x = (screenSize.wd - wd) / 2.0f;
			y = 0.0f;

			if (bars) {
				bars->emplace_back(0.0f, 0.0f, x, hgt);
				bars->emplace_back(static_cast<float>(screenSize.wd)-x, 0.0f, x, hgt );
			}
		}

		return sf::Transform().scale(
			scale, scale
		).translate(
			x / scale, y / scale
		);
	}
};