#pragma once

#include <memory>
#include <unordered_map>
#include "types.hpp"
#include "texture.hpp"
#include "spriteframe.hpp"

namespace ceph {
	class Sprite;
	class SpriteSheet : public std::enable_shared_from_this<SpriteSheet> {
		friend class FontSheet;
	protected:

		struct FrameInfo {
			std::string name;
			ceph::Rect<int> rect;
			int index;

			FrameInfo()
			{}

			FrameInfo(const std::string& n, const ceph::Rect<int>& r, int i) :
				name(n), rect(r), index(i)
			{}
		};

		std::shared_ptr<Texture> texture_;
		std::unordered_map<std::string, FrameInfo> atlas_;
		bool inverted_y_;

		void parseAtlasJson(const std::string& atlas_path);
		SpriteSheet(const std::string& text_path, const std::string& atlas_path, bool invert_y);
		SpriteSheet(const std::shared_ptr<Texture>& tex, const std::unordered_map<std::string, FrameInfo>&& atlas);
		SpriteSheet();

	public:
		static std::shared_ptr<SpriteSheet> create(const std::string& text_path, const std::string& atlas_path, bool invert_y = false);
		std::shared_ptr<Sprite> getSprite(const std::string& name) const;
		std::shared_ptr<Texture> getTexture() const;
		Rect<int> getFrame(const std::string& name) const;
		Vec2<int> getFrameSize(const std::string& name) const;
		SpriteFrame getSpriteFrame(const std::string& name) const;
	};
}