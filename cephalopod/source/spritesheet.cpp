#include <unordered_map>
#include <fstream>
#include <sstream>
#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/spritesheet.hpp"
#include "../include/cephalopod/image.hpp"
#include "json11.hpp"

namespace {
	ceph::Rect<int>  InvertRectVertically(int hgt, ceph::Rect<int> r)
	{
		return ceph::Rect<int>(r.x, hgt - r.y - r.hgt, r.wd, r.hgt);
	}
}

ceph::Rect<int> extractRect(const json11::Json& json) {
	if (!json.is_array() || json.array_items().size() != 4)
		throw std::exception("Invalid rectangle");
	auto& items = json.array_items();
	std::vector<int> nums;
	for (auto& item : items) {
		if (! item.is_number() )
			throw std::exception("Invalid rectangle");
		nums.push_back(item.int_value());
	}
	return ceph::Rect<int>(nums[0], nums[1], nums[2], nums[3]);
}

void ceph::SpriteSheet::parseAtlasJson(const std::string& atlas_path)
{
	std::ifstream t(atlas_path);
	std::stringstream buffer;
	buffer << t.rdbuf();

	std::string err;
	json11::Json json = json11::Json::parse( buffer.str(), err );
	if (!err.empty()) {
		std::string msg = std::string("bad sprite sheet atlas : ") + err;
		throw std::exception( msg.c_str() );
	}
    if (!json.is_object())
		throw std::exception("bad sprite sheet atlas : expected toplevel object");

	for (auto& item : json.object_items()) {
		auto items = item.second.object_items();

		int index = (items.find("index") != items.end()) ?
			items.find("index")->second.int_value() :
			-1;

		ceph::Rect<int> rect = (items.find("rect") != items.end()) ?
			extractRect(items.find("rect")->second) :
			ceph::Rect<int>(0, 0, 0, 0);

		SpriteSheet::FrameInfo frame_info(item.first, rect, index);
		atlas_[item.first] = frame_info;
	}
}

ceph::SpriteSheet::SpriteSheet(const std::string& tex_path, const std::string& atlas_path, bool invert_y) : inverted_y_(invert_y)
{
	texture_ = std::make_shared<ceph::Texture>(tex_path);
	parseAtlasJson(atlas_path);
	if (inverted_y_) {
		int tex_hgt = texture_->getSize().y;
		for (auto& item : atlas_)
			item.second.rect = InvertRectVertically(tex_hgt, item.second.rect);
	}
}

std::shared_ptr<ceph::SpriteSheet> ceph::SpriteSheet::create(const std::string& text_path, const std::string& atlas_path, bool invert_y)
{
	return std::shared_ptr<ceph::SpriteSheet>(new ceph::SpriteSheet(text_path, atlas_path, invert_y));
}

std::shared_ptr<ceph::Sprite> ceph::SpriteSheet::getSprite(const std::string& name) const
{
	return ceph::Actor::create<ceph::Sprite>(shared_from_this(), name);
}

std::shared_ptr<ceph::Texture> ceph::SpriteSheet::getTexture() const
{
	return texture_;
}

ceph::Rect<int> ceph::SpriteSheet::getFrame(const std::string & name) const
{
	return atlas_.find(name)->second.rect;
}


ceph::Vec2<int> ceph::SpriteSheet::getFrameSize(const std::string& name) const
{
	return getFrame(name).getSize();
}

ceph::SpriteFrame ceph::SpriteSheet::getSpriteFrame(const std::string & name) const
{
	return SpriteFrame(texture_, this->getFrame(name));
}

/*
std::tuple<ceph::Rect<int>, int> getFrameInfo(const json11::Json& json)
{
	if (!json.is_object())
		throw std::exception("expected frame info to be an object");
	auto& items = json.object_items();

	int i = (items.find("index") != items.end()) ?
		items.find("index")->second.int_value() :
		-1;
	ceph::Rect<int> r = (items.find("rect") != items.end()) ?
		extractRect(items.find("rect")->second) :
		ceph::Rect<int>(0, 0, 0, 0);

	name = n;
	rect = r;
	index = i;
}
*/