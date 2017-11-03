#include <unordered_map>
#include <fstream>
#include <sstream>
#include "../include/cephalopod/sprite.hpp"
#include "../include/cephalopod/types.hpp"
#include "../include/cephalopod/spritesheet.hpp"
#include "TextureImpl.hpp"
#include "json11.hpp"

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

struct ceph::SpriteSheet::FrameInfo{
	std::string name;
	ceph::Rect<int> rect;
	int index;

	ceph::SpriteSheet::FrameInfo(const std::string& n, const json11::Json& json) {
		if (!json.is_object())
			throw std::exception("expected frame info to be an object");
		auto& items = json.object_items();

		int i = (items.find("index") != items.end()) ?
			items.find("index")->second.int_value() :
			-1;
		Rect<int> r = (items.find("rect") != items.end()) ?
			extractRect(items.find("rect")->second) :
			Rect<int>(0,0,0,0);

		name = n;
		rect = r;
		index = i;
	}
};

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
		atlas_[item.first] = std::make_shared<ceph::SpriteSheet::FrameInfo>(item.first, item.second);
	}
}

ceph::SpriteSheet::SpriteSheet(const std::string& tex_path, const std::string& atlas_path)
{
	texture_ = std::make_shared<ceph::Texture>( tex_path );
	parseAtlasJson(atlas_path);
}

std::shared_ptr<ceph::SpriteSheet> ceph::SpriteSheet::create(const std::string& text_path, const std::string& atlas_path)
{
	return std::shared_ptr<ceph::SpriteSheet>(new ceph::SpriteSheet(text_path, atlas_path));
}

std::shared_ptr<ceph::Sprite> ceph::SpriteSheet::getSprite(const std::string& name) const
{
	return std::make_shared<ceph::Sprite>(shared_from_this(), name);
}

std::shared_ptr<ceph::Texture> ceph::SpriteSheet::getTexture() const
{
	return texture_;
}

ceph::Rect<int> ceph::SpriteSheet::getFrame(const std::string & name) const
{
	return atlas_.find(name)->second->rect;
}

ceph::SpriteFrame ceph::SpriteSheet::getSpriteFrame(const std::string & name) const
{
	return SpriteFrame(texture_, this->getFrame(name));
}
