#pragma once

#include <string>
#include <memory>
#include "fontsheet.hpp"
#include "types.hpp"

namespace ceph {

	class FontImpl;

	class Font
	{
	private:
		std::unique_ptr<ceph::FontImpl> impl_;

	public:

		struct Metrics {
			int ascent;
			int descent;
			int line_gap;

			Metrics(int a = 0, int d = 0, int lg = 0 ) :
				ascent(a), descent(d), line_gap(lg)
			{}
		};

		Font(const std::string& path);
		Rect<int> getCharacterBoundingBox(char ch, float horz_scale, float vert_scale) const;
		void paintGlyph(char ch, unsigned char* data_ptr, int wd, int hgt, int data_stride, float scale) const;
		float getScaleForPixelHeight(int hgt) const;
		Metrics getMetrics() const;
		int getCharacterAdvance(char ch) const;
		int getKernAdvance(char c1, char c2) const;
		std::string getName() const;
		~Font();
	};

}