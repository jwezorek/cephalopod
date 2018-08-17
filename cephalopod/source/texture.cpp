#include "..\include\cephalopod\texture.hpp"
#include "..\include\cephalopod\image.hpp"
#include "glad.h"

ceph::Texture::Texture(const ceph::Image& img, bool useSmoothing, bool useRepeat)
{
	wd_ = img.getWidth();
	hgt_ = img.getHeight();

	static GLenum format[] = { 0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB , GL_RGBA };
	int channels = img.getChannels();
	if (channels < 1 || channels > 4)
		throw std::runtime_error("invalid image");

	GLint filter = (useSmoothing) ? GL_LINEAR : GL_NEAREST;
	GLint wrapMode = (useRepeat) ? GL_REPEAT : GL_CLAMP_TO_EDGE;

	glGenTextures(1, &id_);
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		format[channels],
		static_cast<GLsizei>(wd_),
		static_cast<GLsizei>(hgt_),
		0,
		format[channels],
		GL_UNSIGNED_BYTE,
		img.getData()
	);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int ceph::Texture::getWidth() const
{
	return wd_;
}

int ceph::Texture::getHeight() const
{
	return hgt_;
}

ceph::Rect<int> ceph::Texture::getBounds() const
{
	return ceph::Rect<int>(0, 0, wd_, hgt_);
}

ceph::Vec2<int> ceph::Texture::getSize() const
{
	return ceph::Vec2<int>(wd_, hgt_);
}

void ceph::Texture::bind()
{
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, id_ );
}

ceph::Texture::~Texture()
{
	glDeleteTextures(1, &id_);
}