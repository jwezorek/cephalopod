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

namespace ceph
{
	class FontSheetCell : public ceph::Rect<int>
	{
	public:
		FontSheet::FontItem font_item;
		float scale;
		char character;

		FontSheetCell(const FontSheet::FontItem& fi, float sc = 0, char ch = 0, int w = 0, int h = 0) :
			font_item(fi),
			ceph::Rect<int>(0, 0, w, h),
			scale(sc),
			character(ch)
		{}
	};
}

namespace {

	std::string GetPrintableCharacters();
	std::string g_characters = GetPrintableCharacters();

	std::string GetFontItemKey(const std::string& font_name, int size, char character)
	{
		return (size > 0) ?
			font_name + "_" + std::to_string(size) + "_" + std::string(1, character) :
			font_name + "_" + std::string(1, character);
	}

	std::string GetFontItemKey(const std::string& font_key, char character)
	{
		return font_key + "_" + std::string(1, character);
	}

	std::string GetFontItemKey(const ceph::FontSheetCell& fc, char character)
	{
		return (fc.font_item.name.empty()) ?
			GetFontItemKey(fc.font_item.font->getName(), fc.font_item.size, character) :
			GetFontItemKey(fc.font_item.name, character);
	}

	std::string GetFontKey(const std::string& font_key, const std::string& font_name, int size)
	{
		return (!font_key.empty()) ?
			font_key :
			font_name + "_" + std::to_string(size);
	}

	std::vector<ceph::FontSheetCell> GetFontItemCells(const std::vector<ceph::FontSheet::FontItem>& fonts)
	{
		std::vector<ceph::FontSheetCell> cells;
		cells.reserve(g_characters.size() * fonts.size());
		for (const ceph::FontSheet::FontItem& fi : fonts) {
			float scale = fi.font->getScaleForPixelHeight(fi.size);
			std::transform(g_characters.begin(), g_characters.end(), std::back_insert_iterator(cells),
				[&fi, scale](char ch) {
					auto sz = fi.font->getCharacterBoundingBox(ch, scale, scale).getSize();
					return ceph::FontSheetCell(fi, scale, ch, sz.x + 2, sz.y + 2); // pad by two pixels
				}
			);
		}
		return cells;
	}

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

	std::vector<unsigned char> PaintFontSheetTexture(const ceph::Vec2<int>& tex_sz, const std::vector<ceph::FontSheetCell>& cells)
	{
		std::vector<unsigned char> tex_data;

		size_t data_sz = tex_sz.x * tex_sz.y;
		tex_data.reserve(data_sz * 4);
		tex_data.resize(data_sz);

		for (const auto& cell : cells) {
			unsigned char* data_ptr = &tex_data[cell.x + cell.y * tex_sz.x];
			cell.font_item.font->paintGlyph(cell.character, data_ptr, cell.wd - 1, cell.hgt - 1, tex_sz.x, cell.scale);
		}
		MakeRgbaFromSingleChannel(tex_data);
		stbi_write_png("test-sprite-sheet.png", tex_sz.x, tex_sz.y, 4, &(tex_data[0]), 4 * tex_sz.x);

		return tex_data;
	}
}

ceph::FontSheet::FontItem::FontItem(const std::string& n, const std::shared_ptr<Font>& f, const int sz) :
	name(n),
	font(f),
	size(sz)
{}

ceph::FontSheet::FontItem::FontItem(const std::shared_ptr<Font>& fp, const int sz) :
	FontItem( std::string(), fp, sz)
{ }

std::unordered_map<std::string, ceph::SpriteSheet::FrameInfo> ceph::FontSheet::GetFontAtlas(const std::vector<ceph::FontSheetCell>& cells) const
{
	std::unordered_map<std::string, SpriteSheet::FrameInfo> atlas;

	int index = 0;
	std::transform(cells.begin(), cells.end(), std::inserter(atlas, atlas.end()),
		[&index](const  ceph::FontSheetCell& cell) {
			std::string key = GetFontItemKey(cell, cell.character);
			SpriteSheet::FrameInfo frame_info(key, cell, index++);
			return std::pair<std::string, SpriteSheet::FrameInfo>(key, frame_info);
		}
	);

	return atlas;
}

ceph::FontSheet::FontSheet(const std::vector<FontItem>& fonts)
{
	Vec2<int> tex_sz;

	// generate a cell for each FontItem/printable-character pair ...
	// fill these cells with the correct width and height...
	auto cells = GetFontItemCells(fonts);

	// run sprite packing to fill in locations for the cells.
	ceph::PackSprites(cells, tex_sz);
	
	// we've already padded by two pixels so let's make the cells have one pixel
	// of slop.
	for (auto& cell : cells) {
		cell.wd -= 1;
		cell.hgt -= 1;
	}

	// paint the cells into a single channel image and then convert
	// to RGBA.
	auto tex_data = PaintFontSheetTexture(tex_sz, cells);

	sheet_ = std::shared_ptr<SpriteSheet>(
		new SpriteSheet(
			std::make_shared<ceph::Texture>(ceph::Image(tex_sz.x, tex_sz.y, 4, std::move(tex_data))),
			GetFontAtlas(cells)
		)
	);

	std::transform(fonts.begin(), fonts.end(), std::inserter(fonts_, fonts_.end()),
		[](const FontItem& fi) {
			return std::pair<std::string, FontItem>(
				GetFontKey(fi.name, fi.font->getName(), fi.size), 
				fi
			);
		}
	);
}

std::shared_ptr<ceph::Sprite> ceph::FontSheet::getSprite(const std::string& font, int size, char ch) const
{
	return ceph::Actor::create<ceph::Sprite>( sheet_, GetFontItemKey(font, size, ch));
}

ceph::Rect<int> ceph::FontSheet::getFrame(const std::string& font, int size, char ch) const
{
	return sheet_->getFrame( GetFontItemKey(font, size, ch) );
}

ceph::Vec2<int> ceph::FontSheet::getFrameSize(const std::string& font, int size, char ch) const
{
	return  sheet_->getFrameSize( GetFontItemKey(font, size, ch) );
}

ceph::SpriteFrame ceph::FontSheet::getSpriteFrame(const std::string& font, int size, char ch) const
{
	return  sheet_->getSpriteFrame( GetFontItemKey(font, size, ch) );
}


std::shared_ptr<ceph::Sprite> ceph::FontSheet::getSprite(const std::string& font_key, char ch) const
{
	return ceph::Actor::create<ceph::Sprite>(sheet_, GetFontItemKey(font_key, ch));
}

ceph::Rect<int> ceph::FontSheet::getFrame(const std::string& font_key, char ch) const
{
	return sheet_->getFrame(GetFontItemKey(font_key, ch));
}

ceph::Vec2<int> ceph::FontSheet::getFrameSize(const std::string& font_key, char ch) const
{
	return  sheet_->getFrameSize(GetFontItemKey(font_key, ch));
}

ceph::SpriteFrame ceph::FontSheet::getSpriteFrame(const std::string& font_key, char ch) const
{
	return  sheet_->getSpriteFrame(GetFontItemKey(font_key, ch));
}

ceph::FontSheet::FontItem ceph::FontSheet::getFont(const std::string& font_key) const
{
	return fonts_.find(font_key)->second;
}

ceph::FontSheet::FontItem ceph::FontSheet::getFont(const std::string& font_name, int size) const
{
	return fonts_.find( font_name + "_" + std::to_string(size) )->second;
}


