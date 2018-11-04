#pragma once

#include "spritesheet.hpp"
#include <vector>
#include <memory>

namespace ceph
{
	class Font;
	class FontSheetCell;

	class FontSheet : protected SpriteSheet
	{
	public:

		struct FontItem
		{
			std::string name;
			std::shared_ptr<ceph::Font> font;
			int size;

			FontItem(const std::string& n, const std::shared_ptr<Font>& f, const int sz);
			FontItem(const std::shared_ptr<Font>& fp, const int sz = 24);
		};

		FontSheet(const std::vector<FontItem>& fonts);
		std::shared_ptr<Sprite> getSprite(const std::string& font_key, int size, char ch) const;
		Rect<int> getFrame(const std::string& font_key, int size, char ch) const;
		Vec2<int> getFrameSize(const std::string& font_key, int size, char ch) const;
		SpriteFrame getSpriteFrame(const std::string& font_key, int size, char ch) const;

	private:
		std::unordered_map<std::string, SpriteSheet::FrameInfo> GetFontAtlas(const std::vector<FontSheetCell>& cells) const;
		std::unordered_map<std::string, std::shared_ptr<Font>> fonts_;
	};
}