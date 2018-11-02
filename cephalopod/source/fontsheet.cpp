#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/fontsheet.hpp"
#include "../include/cephalopod/font.hpp"
#include "spritepacker.hpp"
#include "stb_truetype.h"

namespace {

	class FontSheetCell : public ceph::Rect<int>
	{
	public:
		std::string key;

		FontSheetCell(const std::string& k = "", int w = 0, int h = 0) :
			ceph::Rect<int>(0, 0, w, h), 
			key(k)
		{}
	};

	std::string GetPrintableCharacters()
	{
		char chars[255];
		int count = 0;
		for (int i = 0; i <= 255; i++)
			if (std::isprint(i))
				chars[count++] = i;
		return std::string(chars, count);
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
	std::vector<FontSheetCell> cells(g_characters.size());

	for (const ceph::FontSheet::FontItem& fi : fonts) {
		float scale = fi.font->getScaleForPixelHeight(fi.size);
		std::transform(g_characters.begin(), g_characters.end(), cells.begin(),
			[&fi, scale](char ch) {
				auto sz = fi.font->getGlyphSize(ch, scale, scale);
				return FontSheetCell();
			}
		);
	}
	return nullptr;
}

ceph::FontSheet::FontSheet(const std::vector<FontItem>& fonts) 
{
	texture_ = GenerateTexture(fonts);
}


