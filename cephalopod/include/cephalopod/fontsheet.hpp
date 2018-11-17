#pragma once

#include "spritesheet.hpp"
#include <vector>
#include <memory>

namespace ceph
{
	class Font;
	class FontSheetCell;

	class FontSheet
	{
	public:

		struct FontItem
		{
			std::string name;
			std::shared_ptr<ceph::Font> font;
			int size;

			FontItem(const std::string& n, const std::shared_ptr<Font>& f, const int sz);
			FontItem(const std::shared_ptr<Font>& fp, const int sz);
		};

		FontSheet(const std::vector<FontItem>& fonts);

		std::shared_ptr<Sprite> getSprite(const std::string& font, int size, char ch) const;
		Rect<int> getFrame(const std::string& font, int size, char ch) const;
		Vec2<int> getFrameSize(const std::string& font, int size, char ch) const;
		SpriteFrame getSpriteFrame(const std::string& font, int size, char ch) const;

		std::shared_ptr<Sprite> getSprite(const std::string& font_key, char ch) const;
		Rect<int> getFrame(const std::string& font_key, char ch) const;
		Vec2<int> getFrameSize(const std::string& font_key, char ch) const;
		SpriteFrame getSpriteFrame(const std::string& font_key, char ch) const;

		FontItem getFont(const std::string& font_key) const;
		FontItem getFont(const std::string& font_name, int size) const;

	private:
		std::unordered_map<std::string, SpriteSheet::FrameInfo> GetFontAtlas(const std::vector<FontSheetCell>& cells) const;
		std::unordered_map<std::string, FontItem> fonts_;
		std::shared_ptr<SpriteSheet> sheet_;
	};
}