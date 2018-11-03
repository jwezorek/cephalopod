#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/fontsheet.hpp"
#include "../include/cephalopod/font.hpp"
#include "../include/cephalopod/Image.hpp"
#include "../include/cephalopod/Texture.hpp"
#include "spritepacker.hpp"
#include "stb_truetype.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" /* http://nothings.org/stb/stb_image_write.h */

namespace {

	class FontSheetCell : public ceph::Rect<int>
	{
	public:
		std::shared_ptr<ceph::Font> font;
		float scale;
		char character;

		FontSheetCell(const std::shared_ptr<ceph::Font>& fnt = nullptr, float sc = 0, char ch = 0, int w = 0, int h = 0) :
			ceph::Rect<int>( 0, 0, w, h ), 
			font( fnt ),
			scale( sc ),
			character( ch )
		{}
	};

	std::string GetPrintableCharacters()
	{
		char chars[255];
		int count = 0;
		for (int i = 0; i <= 255; i++)
			if (std::isgraph(i))
				chars[count++] = i;
		return std::string(chars, count);
	}

	void MakeRgbaFromSingleChannel(std::vector<unsigned char>& data)
	{
		size_t src_sz = data.size();
		size_t dst_sz = src_sz * 4;
		data.resize(dst_sz);
		unsigned char *src, *dst;
		for (src = &data[src_sz - 1], dst = &data[dst_sz - 4]; dst >= &data[0]; src--, dst -= 4)
		{
			dst[0] = dst[1] = dst[2] = (*src > 0) ? 255 : 0;
			dst[3] = *src;
		}
	}

	std::string g_characters = GetPrintableCharacters();

}

ceph::FontSheet::FontItem::FontItem(const std::string& n, const std::shared_ptr<Font>& f, const int sz) :
	name(n),
	font(f),
	size(sz)
{}

ceph::FontSheet::FontItem::FontItem(const std::shared_ptr<Font>& fp, const int sz) :
	FontItem(fp->getName(), fp, sz)
{ }

std::shared_ptr<ceph::Texture> ceph::FontSheet::GenerateTexture(const std::vector<ceph::FontSheet::FontItem>& fonts)
{
	std::vector<FontSheetCell> cells;
	cells.reserve(g_characters.size() * fonts.size());

	for (const ceph::FontSheet::FontItem& fi : fonts) {
		float scale = fi.font->getScaleForPixelHeight(fi.size);
		std::transform(g_characters.begin(), g_characters.end(), std::back_insert_iterator(cells),
			[&fi, scale](char ch) {
				auto sz = fi.font->getGlyphSize(ch, scale, scale);
				return FontSheetCell( fi.font, scale, ch, sz.x+1, sz.y+1);
			}
		);
	}

	Vec2<int> tex_sz;
	ceph::PackSprites( cells, tex_sz );

	std::vector<unsigned char> tex_data;
	size_t data_sz = tex_sz.x * tex_sz.y;
	tex_data.reserve(data_sz * 4);
	tex_data.resize(data_sz);

	for (const auto& cell : cells) {
		unsigned char* data_ptr = &tex_data[cell.x + cell.y * tex_sz.x];
		cell.font->paintGlyph(cell.character, data_ptr, cell.wd-1, cell.hgt-1, tex_sz.x, cell.scale);
	}

	MakeRgbaFromSingleChannel(tex_data);
	stbi_write_png("out-4.png", tex_sz.x, tex_sz.y, 4, &(tex_data[0]), 4 * tex_sz.x);

	return std::make_shared<ceph::Texture>( ceph::Image( tex_sz.x, tex_sz.y, 4, std::move(tex_data) ) );
}

ceph::FontSheet::FontSheet(const std::vector<FontItem>& fonts) 
{
	texture_ = GenerateTexture(fonts);
}


