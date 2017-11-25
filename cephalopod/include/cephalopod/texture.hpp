#pragma once

#include <memory>
#include <string>
#include "types.hpp"

namespace ceph
{
	class TextureImpl;

	class Texture 
	{
		friend class SpriteImpl;
		friend class SceneImpl;
	private:
		std::unique_ptr<TextureImpl> impl_;
	public:
		Texture();
		Texture(const std::string& filepath, bool invert_y_ = false);
		Size<int> getSize() const;
		Rect<int> getBounds() const;
		~Texture();
	};
}