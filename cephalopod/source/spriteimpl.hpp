#pragma once

#include <memory>
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/spriteframe.hpp"
#include "SFML/Graphics.hpp"

namespace ceph
{
	class SpriteImpl
	{
	public:
		sf::Sprite sfml_sprite_;
		sf::Vector2f anchor_;
		SpriteImpl();
		SpriteImpl(const std::shared_ptr<ceph::Texture>& texture);
		SpriteImpl(const std::shared_ptr<ceph::Texture>& texture, const ceph::Rect<int>& r);
		SpriteImpl(const ceph::SpriteFrame& frame );
	};
}