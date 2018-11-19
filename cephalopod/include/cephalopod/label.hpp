#pragma once

#include "group.hpp"
#include "fontsheet.hpp"
#include <string>
#include <memory>

namespace ceph
{
	class Label : public Group
	{
	public:

		enum class Justification {
			Left,
			Center,
			Right
		};

		Label(const std::shared_ptr<FontSheet> fs, const std::string& font_name, int font_sz, const std::string& text, Justification just = Justification::Left);
		Label(const std::shared_ptr<FontSheet> fs, const std::string& font_key, const std::string& text, Justification just = Justification::Left);
		virtual void initialize() override;

		std::string getText() const;
		void setText(const std::string& text);

		void setFont(const std::string& key);
		void setFont(const std::string& fontname, int sz);

		Justification getJustification() const;
		void setJustification(Justification just);

		std::shared_ptr<FontSheet> getFontSheet() const;

	private:
		std::shared_ptr<FontSheet> font_sheet_;
		std::string font_key_str_;
		int font_key_int_;
		std::string text_;
		Justification just_;
	};
}