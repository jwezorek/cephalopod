#include "button.hpp"
#include "guiwidget.hpp"

namespace {
	enum ButtonFrame
	{
		Normal = 0,
		Selected,
		Clicked,
		Disabled
	};

	ButtonFrame GetFrame(bool is_clicked, bool is_enabled, bool is_selected)
	{
		if (!is_enabled)
			return Disabled;
		if (is_clicked)
			return Clicked;
		if (is_selected)
			return Selected;
		return Normal;
	}
}

ceph::Button::Button(const std::shared_ptr<SpriteSheet>& sheet, const std::string& normal_frame, 
		const std::string & selected_frame, const std::string & clicked_frame, const std::string & disabled_ftame) :
	ceph::Sprite( sheet, normal_frame ),
	is_clicked_( false )
{
	frames_[ButtonFrame::Normal] = normal_frame;
	frames_[ButtonFrame::Selected] = selected_frame;
	frames_[ButtonFrame::Clicked] = clicked_frame;
	frames_[ButtonFrame::Disabled] = disabled_ftame;
}

void ceph::Button::handleKeyDown(ceph::KeyCode key, ceph::KeyModifiers modifiers)
{
	if (key == ceph::KeyCode::Space) {
		is_clicked_ = true;
		onStateChange();
	}
}

void ceph::Button::handleKeyUp(ceph::KeyCode key, ceph::KeyModifiers modifiers)
{
	if (key == ceph::KeyCode::Space) {
		is_clicked_ = false;
		onStateChange();
		clickEvent.fire(*this);
	}
}

void ceph::Button::onStateChange()
{
	setFrame(
		frames_[ GetFrame(is_clicked_, isEnabled(), hasFocus()) ]
	);
}

void ceph::Button::onAttachScene()
{
	onWidgetAttachedToScene();
}

void ceph::Button::onBeforeDetachScene()
{
	onBeforeWidgetDetachedFromScene();
}
