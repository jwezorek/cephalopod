#pragma once

#include <memory>
#include "types.hpp"
#include "texture.hpp"

namespace ceph
{
	class SpriteFrame
	{
	private:
		std::shared_ptr<Texture> texture_;
		Rect<int> rect_;
	public:
		SpriteFrame(const std::shared_ptr<Texture>& tex = nullptr, const Rect<int>& rect = Rect<int>(0,0,0,0));
		std::shared_ptr<Texture> getTexture() const;
		Rect<int> getRect() const;
		Vec2<int> getSize() const;
	};
}