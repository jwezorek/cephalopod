#pragma once

#include "spritesheet.hpp"
#include <vector>
#include <memory>

namespace ceph
{
	class Font;

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

	private:
		std::shared_ptr<ceph::Texture> GenerateTexture(const std::vector<FontItem>& fonts);

	public:


		FontSheet(const std::vector<FontItem>& fonts);
	};
}