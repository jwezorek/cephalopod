#include "../include/cephalopod/guiwidget.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/scene.hpp"
#include "guiwidgets.hpp"
#include <iostream>

ceph::GuiWidget::GuiWidget() :
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
	return hasParent() ? parent().getFocus() == this : false;
}

void ceph::GuiWidget::focus()
{
	is_enabled_ = true;
	onStateChange();
}

void ceph::GuiWidget::removeFocus()
{
	if (hasParent()) {
		parent().clearFocus();
		onStateChange();
	}
}

void ceph::GuiWidget::focusNext()
{
	if (!hasParent())
		return;
	auto& widgets = parent();
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
	if (!hasParent())
		return;
	auto& widgets = parent();
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

ceph::GuiWidgets& ceph::GuiWidget::parent()
{
	return dynamic_cast<ceph::Actor*>(this)->getScene().getWidgets();
}

const ceph::GuiWidgets& ceph::GuiWidget::parent() const
{
	return dynamic_cast<const ceph::Actor*>(this)->getScene().getWidgets();
}

bool ceph::GuiWidget::hasParent() const
{
	return dynamic_cast<const ceph::Actor*>(this)->isInScene();
}

void ceph::GuiWidget::onWidgetAttachedToScene()
{
	if (hasParent())
		parent().addWidget(*this);
}

void ceph::GuiWidget::onBeforeWidgetDetachedFromScene()
{
	if (hasParent())
		parent().removeWidget(*this);
}

void ceph::GuiWidget::setNext(const std::shared_ptr<GuiWidget>& widget)
{
	next_ = widget;
}

void ceph::GuiWidget::setPrev(const std::shared_ptr<GuiWidget>& widget)
{
	prev_ = widget;
}

