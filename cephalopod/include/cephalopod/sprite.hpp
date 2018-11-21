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
		ColorRGB tint_; // TODO: move tint into ActorState and have a tint action

	protected:

		virtual void drawThis(DrawingContext& rt) const;
		//Sprite(const std::shared_ptr<Texture>& texture);
		Sprite(const std::shared_ptr<const SpriteSheet>& sheet, const std::string& frame_name);

	public:
		void setTint(ColorRGB color); 
		ColorRGB getTint() const;
		std::string getFrame() const;
		void setFrame(const std::string& frame_name);
		virtual ~Sprite();
	};
}
