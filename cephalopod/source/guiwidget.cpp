#include "../include/cephalopod/guiwidget.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/scene.hpp"
#include "guiwidgets.hpp"
#include <iostream>

ceph::GuiWidget::GuiWidget(ceph::Actor& self) :
	self_(self),
	is_enabled_(false)
{
}

bool ceph::GuiWidget::isEnabled() const
{
	return is_enabled_;
}

void ceph::GuiWidget::enable()
{
	is_enabled_ = true;
	onStateChange();
}

void ceph::GuiWidget::disable()
{
	is_enabled_ = false;
	onStateChange();
}

bool ceph::GuiWidget::hasFocus() const
{
	auto& widgets = self_.getScene().getWidgets();
	return widgets.getFocus() == this;
}

void ceph::GuiWidget::focus()
{
	is_enabled_ = true;
	onStateChange();
}

void ceph::GuiWidget::removeFocus()
{
	auto& widgets = self_.getScene().getWidgets();
	widgets.clearFocus();
	onStateChange();
}

void ceph::GuiWidget::focusNext()
{
	auto& widgets = self_.getScene().getWidgets();
	if (!next_.expired()) {
		auto next = next_.lock();
		widgets.setFocus(*next);
	} else {
		widgets.clearFocus();
	}
	onStateChange();
}

void ceph::GuiWidget::focusPrev()
{
	auto& widgets = self_.getScene().getWidgets();
	if (!prev_.expired()) {
		auto prev = prev_.lock();
		widgets.setFocus(*prev);
	} else {
		widgets.clearFocus();
	}
	onStateChange();
}

ceph::GuiWidget* ceph::GuiWidget::getNext()
{
	return (!next_.expired()) ? next_.lock().get() : nullptr;
}

ceph::GuiWidget * ceph::GuiWidget::getPrev()
{
	return (!prev_.expired()) ? prev_.lock().get() : nullptr;
}

void ceph::GuiWidget::onWidgetAttachedToScene()
{
	auto& widgets = self_.getScene().getWidgets();
	widgets.addWidget(*this);
}

void ceph::GuiWidget::onBeforeWidgetDetachedFromScene()
{
	auto& widgets = self_.getScene().getWidgets();
	widgets.removeWidget(*this);
}

void ceph::GuiWidget::setNext(const std::shared_ptr<GuiWidget>& widget)
{
	next_ = widget;
}

void ceph::GuiWidget::setPrev(const std::shared_ptr<GuiWidget>& widget)
{
	prev_ = widget;
}

