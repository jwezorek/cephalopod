#include <cctype>
#include "..\include\cephalopod\label.hpp"
#include "..\include\cephalopod\font.hpp"
#include "..\include\cephalopod\sprite.hpp"

namespace {

	struct LineInfo
	{
		int start_index;
		int end_index;
		float wd;

		LineInfo(int s, int e, float w) :
			start_index(s),
			end_index(e),
			wd(w)
		{}
	};

	std::vector<LineInfo> GetLines(const ceph::Font& font, const std::string& text, float scale)
	{
		auto get_line_fn = [&](int start_index) -> LineInfo {
			float x = 0;
			int i = start_index;
			while (i+1 < text.size() && text[i+1] != '\n') {
				x += scale * (font.getCharacterAdvance(text[i]) + font.getKernAdvance(text[i], text[i + 1]));
				i++;
			}
			return LineInfo(
				start_index,
				i + 1,
				x + font.getCharacterBoundingBox(text[i], scale, scale).wd
			);
		};
		
		std::vector<LineInfo> lines;
		int i = 0;
		while (i < text.size()) {
			auto line = get_line_fn(i);
			lines.push_back(line);
			i = line.end_index + 1;
		}

		return lines;
	}

}

ceph::Label::Label(const std::shared_ptr<FontSheet> fs, const std::string & font_name, int font_sz, const std::string& text, 
		Justification just, const ColorRGB& color) :
	font_sheet_(fs),
	font_key_str_(font_name),
	font_key_int_(font_sz),
	text_(text),
	just_(just),
	color_(color)
{
}

ceph::Label::Label(const std::shared_ptr<FontSheet> fs, const std::string& font_key, const std::string& text, 
		Justification just, const ColorRGB& color) :
	font_sheet_(fs),
	font_key_str_(font_key),
	font_key_int_(0),
	text_(text),
	just_(just),
	color_(color)
{
}

void ceph::Label::initialize()
{
	std::string key = (font_key_int_ > 0) ? font_key_str_ + "_" + std::to_string(font_key_int_) : font_key_str_;
	auto& font_item = font_sheet_->getFont(key);
	const auto& font = *font_item.font;
	float scale = font.getScaleForPixelHeight(font_item.size);
	auto metrics = font.getMetrics();

	float ascent = metrics.ascent * scale;
	float descent = metrics.descent * scale;
	auto lines = GetLines(font, text_, scale);

	float line_y = 0;
	for (const auto& line : lines) {
		float x = 0;
		if (just_ != Justification::Left)
			x = (just_ == Justification::Right) ? -line.wd : -(line.wd / 2.0f);
		for (int i = line.start_index; i < line.end_index; i++) {
			char ch = text_[i];
			char next_ch = text_[i + 1];
			float character_y = line_y - font.getCharacterBoundingBox(ch, scale, scale).y2();

			if (std::isgraph(ch)) {
				auto sprite = font_sheet_->getSprite(font_key_str_, font_key_int_, ch);
				sprite->setAnchorPcnt(0, 0);
				float character_x = x + scale * font.getCharacterLeftSideBearing(ch);
				sprite->moveTo(character_x, character_y);
				sprite->setTint(color_);
				addChild(sprite);
			}
			x += scale * (font.getCharacterAdvance(ch) + font.getKernAdvance(ch, next_ch));
		}
		line_y -= (metrics.ascent - metrics.descent + metrics.line_gap) * scale;
	}
}

std::string ceph::Label::getText() const
{
	return text_;
}

void ceph::Label::setText(const std::string & text)
{
	text_ = text;
	removeAllChildren();
	initialize();
}

void ceph::Label::setFont(const std::string &key)
{
	font_key_str_ = key;
	font_key_int_ = 0;
	removeAllChildren();
	initialize();
}

void ceph::Label::setFont(const std::string & fontname, int sz)
{
	font_key_str_ = fontname;
	font_key_int_ = sz;
	removeAllChildren();
	initialize();
}

void ceph::Label::setColor(const ColorRGB& color)
{
	for (auto& child : children_) {
		auto sprite = std::dynamic_pointer_cast<ceph::Sprite>(child);
		if (sprite)
			sprite->setTint(color);
	}
	color_ = color;
}

ceph::ColorRGB ceph::Label::getColor() const
{
	return color_;
}

ceph::Label::Justification ceph::Label::getJustification() const
{
	return just_;
}

void ceph::Label::setJustification(ceph::Label::Justification just)
{
	just_ = just;
	removeAllChildren();
	initialize();
}

std::shared_ptr<ceph::FontSheet> ceph::Label::getFontSheet() const
{
	return font_sheet_;
}







