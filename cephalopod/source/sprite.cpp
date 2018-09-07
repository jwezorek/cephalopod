#include <memory>
#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/game.hpp"
#include "../include/cephalopod/drawingcontext.hpp"
#include "graphics.hpp"

ceph::Sprite::Sprite(const std::shared_ptr<Texture>& texture)
{
}

ceph::Sprite::Sprite(const std::shared_ptr<const SpriteSheet>& sheet, const std::string& frame_name) :
	sprite_sheet_(sheet)
{
	state_.setSpriteSheet(sheet);
	state_.setSpriteFrame(frame_name);
}

void ceph::Sprite::setFrame(const std::string& frame_name)
{
	state_.setSpriteFrame(frame_name);
}

void ceph::Sprite::drawThis(DrawingContext& dc) const
{
	auto curr_tex = dc.graphics.GetCurrentTexture();
	auto sprite_sheet_tex = sprite_sheet_->getTexture();

	if (curr_tex.get() != sprite_sheet_tex.get())
		dc.graphics.SetCurrentTexture(sprite_sheet_tex);

	auto sprite_sz = state_.getSize();
	auto matrix = dc.transformation * ceph::Mat3x3().scale(static_cast<float>(sprite_sz.x), static_cast<float>(sprite_sz.y));
	dc.graphics.Blit(matrix, state_.getRect(), dc.alpha);
}

ceph::Sprite::~Sprite()
{
}
