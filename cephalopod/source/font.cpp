#include "..\include\cephalopod\font.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb_truetype.h" /* http://nothings.org/stb/stb_truetype.h */

namespace
{
	std::vector<unsigned char> ReadFile(const std::string& filename)
	{
		// open the file:
		std::ifstream file(filename, std::ios::binary);

		// read the data:
		return std::vector<unsigned char>(
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()
		);
	}
}

namespace ceph {
	class FontImpl
	{
	public:

		std::vector<unsigned char> data;
		stbtt_fontinfo info;
		std::string fname;

		FontImpl(const std::string& path) {

			std::filesystem::path p(path);
			fname = p.filename().string();

			data = ReadFile(path);
			if (!stbtt_InitFont(&info, &(data[0]), 0))
				throw std::runtime_error("unable to load font: " + path);
		}
	};
}

ceph::Font::Font(const std::string& path)
{
	impl_ = std::make_unique<ceph::FontImpl>(path);
}

ceph::Rect<int> ceph::Font::getCharacterBoundingBox(char ch, float horz_scale, float vert_scale) const
{
	int c_x1, c_y1, c_x2, c_y2;
	stbtt_GetCodepointBitmapBox(&(impl_->info), ch, horz_scale, vert_scale, &c_x1, &c_y1, &c_x2, &c_y2);
	return {  c_x1,  c_y1, c_x2 - c_x1, c_y2 - c_y1 };
}

void ceph::Font::paintGlyph(char ch, unsigned char * data_ptr, int wd, int hgt, int data_stride, float scale) const
{
	stbtt_MakeCodepointBitmap( &(impl_->info), data_ptr, wd, hgt, data_stride, scale, scale, ch );
}

float ceph::Font::getScaleForPixelHeight(int hgt) const
{
	return stbtt_ScaleForPixelHeight(
		&(impl_->info), 
		static_cast<float>(hgt)
	);
}

ceph::Font::Metrics ceph::Font::getMetrics() const
{
	ceph::Font::Metrics metrics;
	stbtt_GetFontVMetrics(
		&(impl_->info), 
		&metrics.ascent, 
		&metrics.descent,
		&metrics.line_gap
	);
	return metrics;
}

int ceph::Font::getCharacterAdvance(char ch) const
{
	int ax = 0;
	stbtt_GetCodepointHMetrics(&(impl_->info), ch, &ax, 0);
	return ax;
}

int ceph::Font::getKernAdvance(char c1, char c2) const
{
	return  stbtt_GetCodepointKernAdvance(&(impl_->info), c1, c2);
}

ceph::Font::~Font()
{
}

std::string ceph::Font::getName() const
{
	return impl_->fname;
}
