#pragma once

#include <string>
#include <memory>
#include "fontsheet.hpp"
#include "types.hpp"

namespace ceph {

	class FontImpl;

	class Font
	{
		friend FontSheet;

	private:
		std::unique_ptr<ceph::FontImpl> impl_;

	public:
		Font(const std::string& path);
		Vec2<int> getGlyphSize(char ch, float horz_scale, float vert_scale) const;
		void paintGlyph(char ch, unsigned char* data_ptr, int wd, int hgt, int data_stride, float scale) const;
		float getScaleForPixelHeight(int hgt) const;
		std::string getName() const;
		~Font();
	};

}