#pragma once

#include <memory>
#include <unordered_map>
#include "types.hpp"
#include "texture.hpp"
#include "spriteframe.hpp"

namespace ceph {
	class Sprite;

	class SpriteSheet : public std::enable_shared_from_this<SpriteSheet> {
	private:
		struct FrameInfo;

		std::shared_ptr<Texture> texture_;
		//TODO: there's really no reason for this to have shared_ptr values...
		// not sure why i did that.
		std::unordered_map<std::string, std::shared_ptr<FrameInfo>> atlas_;
		bool inverted_y_;

		void parseAtlasJson(const std::string& atlas_path);
		SpriteSheet(const std::string& text_path, const std::string& atlas_path, bool invert_y);

	public:
		static std::shared_ptr<SpriteSheet> create(const std::string& text_path, const std::string& atlas_path, bool invert_y = false);
		std::shared_ptr<Sprite> getSprite(const std::string& name) const;
		std::shared_ptr<Texture> getTexture() const;
		Rect<int> getFrame(const std::string& name) const;
		SpriteFrame getSpriteFrame(const std::string& name) const;
	};
}