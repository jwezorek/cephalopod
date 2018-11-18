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

ceph::Label::Label(const std::shared_ptr<FontSheet> fs, const std::string & font_name, int font_sz, const std::string& text, Justification just) :
	font_sheet_(fs),
	font_key_(font_name),
	font_sz_(font_sz),
	text_(text),
	just_(just)
{
}

ceph::Label::Label(const std::shared_ptr<FontSheet> fs, const std::string& font_key, const std::string& text, Justification just) :
	font_sheet_(fs),
	font_key_(font_key),
	font_sz_(0),
	text_(text),
	just_(just)
{
}

void ceph::Label::initialize()
{
	std::string key = (font_sz_ > 0) ? font_key_ + "_" + std::to_string(font_sz_) : font_key_;
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
				auto sprite = font_sheet_->getSprite(font_key_, font_sz_, ch);
				sprite->setAnchorPcnt(0, 0);
				float character_x = x + scale * font.getCharacterLeftSideBearing(ch);
				sprite->moveTo(character_x, character_y);
				addChild(sprite);
			}
			x += scale * (font.getCharacterAdvance(ch) + font.getKernAdvance(ch, next_ch));
		}
		line_y -= (metrics.ascent - metrics.descent + metrics.line_gap) * scale;
	}
}

/*
void ceph::Label::initialize()
{
	std::string key = (font_sz_ > 0) ? font_key_ + "_" + std::to_string(font_sz_) : font_key_;
	auto& font_item = font_sheet_->getFont( key );
	const auto& font = *font_item.font;
	float scale = font.getScaleForPixelHeight(font_item.size);
	auto metrics = font.getMetrics();

	float ascent = metrics.ascent * scale;
	float descent = metrics.descent * scale;
	float x = 0, line_y = 0;

	for (int i = 0; i < text_.size(); i++) {
		char ch = text_[i];
		char next_ch = text_[i + 1];

		float character_y = line_y - font.getCharacterBoundingBox(ch, scale, scale).y2();

		if (std::isgraph(ch)) {
			auto sprite = font_sheet_->getSprite(font_key_, font_sz_, ch);
			sprite->setAnchorPcnt(0, 0);
			float character_x = x + scale * font.getCharacterLeftSideBearing(ch);
			sprite->moveTo(character_x, character_y);
			addChild(sprite);
		}

		if (ch != '\n') {
			x += scale * (font.getCharacterAdvance(ch) + font.getKernAdvance(ch, next_ch));
		} else {
			x = 0;
			// should be equal to font_sz_ + line_gap I guess...
			line_y -= (metrics.ascent - metrics.descent + metrics.line_gap) * scale; 
		}
	}
}
*/
