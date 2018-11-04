#include "..\include\cephalopod\label.hpp"
#include "..\include\cephalopod\font.hpp"
#include "..\include\cephalopod\sprite.hpp"

ceph::Label::Label(const std::shared_ptr<FontSheet> fs, const std::string & font_key, int font_sz, const std::string& text) :
	font_sheet_(fs),
	font_key_(font_key),
	font_sz_(font_sz),
	text_(text)
{
}

void ceph::Label::initialize()
{
	auto& font = *font_sheet_->getFont(font_key_);
	float scale = font.getScaleForPixelHeight(font_sz_);
	auto metrics = font.getMetrics();

	float ascent = metrics.ascent * scale;
	float descent = metrics.descent * scale;
	float x = 0;
	for (int i = 0; i < text_.size(); i++) {
		char ch = text_[i];
		char next_ch = (i < text_.size()) ? text_[i + 1] : 0;
		auto y = ascent + font.getCharacterBoundingBox(ch, scale, scale).y;

		auto sprite = font_sheet_->getSprite(font_key_, font_sz_, ch);
		sprite->moveTo(x, y);
		addChild(sprite);

		x += scale * font.getCharacterAdvance(ch);
		x += scale * font.getKernAdvance(ch, next_ch);
	}
}
