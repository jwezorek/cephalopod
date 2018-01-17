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
		SpriteFrame sprite_frame_;
		std::shared_ptr<const SpriteSheet> sprite_sheet_;
		std::unique_ptr<SpriteImpl> impl_;

	protected:

		virtual void drawThis(DrawingContext& rt) const;
		Sprite(const std::shared_ptr<Texture>& texture);
		Sprite(const std::shared_ptr<const SpriteSheet>& sheet, const std::string& frame_name);

	public:

		void setFrame(const std::string& frame_name);

		Vec2D<float> getGlobalPosition() const override;
		void setGlobalPosition(const Vec2D<float>& pt_global) override;
		Rect<float> getLocalBounds() const override;
		Rect<float> getGlobalBounds() const override;

		virtual ~Sprite();
	};
}
