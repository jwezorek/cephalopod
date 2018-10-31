#include <unordered_map>
#include <fstream>
#include <sstream>
#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/fontsheet.hpp"
#include "spritepacker.hpp"

namespace {
	std::shared_ptr<ceph::Texture> GenerateTexture(const std::vector<ceph::FontSheet::FontItem>& fonts)
	{
		return nullptr;
	}
}

ceph::FontSheet::FontSheet(const std::vector<FontItem>& fonts) 
{
	texture_ = GenerateTexture(fonts);
}
