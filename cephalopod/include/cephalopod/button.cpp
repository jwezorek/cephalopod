#include "button.hpp"

namespace {
	enum ButtonFrames
	{
		Normal = 0,
		Selected,
		Clicked,
		Disabled
	};
}


ceph::Button::Button(const std::string& id, const std::shared_ptr<const SpriteSheet>& sheet, const std::string& normal_frame, 
		const std::string & selected_frame, const std::string & clicked_frame, const std::string & disabled_ftame) :
	ceph::Sprite(sheet, normal_frame),
	ceph::GuiWidget(id, *this)
{
	frames_[ButtonFrames::Normal] = normal_frame;
	frames_[ButtonFrames::Selected] = selected_frame;
	frames_[ButtonFrames::Clicked] = clicked_frame;
	frames_[ButtonFrames::Disabled] = disabled_ftame;
}
