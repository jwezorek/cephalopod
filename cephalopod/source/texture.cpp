#include "SFML/Graphics.hpp"
#include "../include/cephalopod/texture.hpp"
#include "textureimpl.hpp"

ceph::Texture::Texture()
{
}

ceph::Texture::Texture(const std::string& filepath, bool invert_y) :
	impl_( std::make_unique<ceph::TextureImpl>() )
{
	if (!invert_y) {
		impl_->sfml_impl_.loadFromFile(filepath);
	} else {
		sf::Image image;
		image.loadFromFile(filepath);
		image.flipVertically();
		impl_->sfml_impl_.loadFromImage(image);
	}

	impl_->sfml_impl_.setSmooth(true);
}

ceph::Texture::~Texture()
{
}

ceph::Size<int> ceph::Texture::getSize() const
{
	auto sz = impl_->sfml_impl_.getSize();
	return ceph::Size<int>(static_cast<int>(sz.x), static_cast<int>(sz.y));
}

ceph::Rect<int> ceph::Texture::getBounds() const
{
	auto sz = impl_->sfml_impl_.getSize();
	return ceph::Rect<int>(0, 0, static_cast<int>(sz.x), static_cast<int>(sz.y));
}
