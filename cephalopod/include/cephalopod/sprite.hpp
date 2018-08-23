#pragma once

#include <memory>
#include "actor.hpp"
#include "types.hpp"
#include "spritesheet.hpp"
#include "texture.hpp"
#include "spriteframe.hpp"
#include "signals.hpp"

namespace ceph {

	class SpriteImpl;

	class Sprite : public Actor
	{
		friend class Actor;

	private:
		std::shared_ptr<const SpriteSheet> sprite_sheet_;

	protected:

		virtual void drawThis(DrawingContext& rt) const;
		Sprite(const std::shared_ptr<Texture>& texture);
		Sprite(const std::shared_ptr<const SpriteSheet>& sheet, const std::string& frame_name);

	public:

		void setFrame(const std::string& frame_name);
		virtual ~Sprite();
	};
}
