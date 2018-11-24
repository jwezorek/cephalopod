#include "..\include\cephalopod\guiwidget.hpp"
#include "..\include\cephalopod\actor.hpp"
#include "..\include\cephalopod\scene.hpp"
#include "guiwidgets.hpp"

ceph::GuiWidget::GuiWidget(ceph::Actor& self) :
	self_(self),
	is_enabled_(false)
{
}

bool ceph::GuiWidget::isEnabled() const
{
	return is_enabled_;
}

void ceph::GuiWidget::Enable()
{
	is_enabled_ = true;
	onStateChange();
}

void ceph::GuiWidget::Disable()
{
	is_enabled_ = false;
	onStateChange();
}

bool ceph::GuiWidget::hasFocus() const
{
	auto& widgets = self_.getScene().getWidgets();
	return widgets.getFocus() == this;
}

void ceph::GuiWidget::SetFocus()
{
	is_enabled_ = true;
	onStateChange();
}
