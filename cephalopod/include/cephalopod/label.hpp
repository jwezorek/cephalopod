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
	private:
		std::shared_ptr<FontSheet> font_sheet_;
		std::string font_key_;
		int font_sz_;
		std::string text_;
		Justification just_;
	};
}