#include "../include/cephalopod/spriteframe.hpp"

ceph::SpriteFrame::SpriteFrame(const std::shared_ptr<ceph::Texture>& tex, const Rect<int>& rect) :
	texture_(tex),
	rect_(rect)
{
	if (texture_ && rect.wd == 0 && rect.hgt == 0)
		rect_ = texture_->getBounds();
}

std::shared_ptr<ceph::Texture> ceph::SpriteFrame::getTexture() const
{
	return texture_;
}

ceph::Rect<int> ceph::SpriteFrame::getRect() const
{
	return rect_;
}

