#pragma once

#include <memory>
#include "types.hpp"

namespace ceph
{
	class Image;

    class Texture
    {
	private:

		int wd_, hgt_;
		unsigned int id_;

	public:
		Texture(const ceph::Image& bitmap, bool useSmoothing = true, bool useRepeat = false);
		int getWidth() const;
		int getHeight() const;
		Rect<int> getBounds() const;
		Vec2<int> getSize() const;
		void bind();
		~Texture();
    };
};
