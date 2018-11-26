#include "guiwidgets.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/guiwidget.hpp"
#include <algorithm>

void ceph::GuiWidgets::addWidget(GuiWidget& widget)
{
	if (widgets_.empty())
		initial_widget_ = &widget;
	widgets_.push_back( &widget );
}

void ceph::GuiWidgets::removeWidget(GuiWidget& widget)
{
	widgets_.erase(std::remove(widgets_.begin(), widgets_.end(), &widget), widgets_.end());
}

void ceph::GuiWidgets::handleKeyEvent(bool isPressed, ceph::KeyCode key, ceph::KeyModifiers modifiers)
{
	if (isPressed)
		handleKeyDown( key, modifiers );
	else
		handleKeyUp( key, modifiers );
}

void ceph::GuiWidgets::handleKeyDown(ceph::KeyCode key, ceph::KeyModifiers modifiers)
{
	if (key == KeyCode::Tab) {
		bool go_to_next = !any(modifiers & ceph::KeyModifiers::Alt);
		if (!focused_widget_) {
			focused_widget_ = initial_widget_;
			focused_widget_->focus();
		}
		else {
			auto new_focus = (go_to_next) ? focused_widget_->getNext() : focused_widget_->getPrev();
			focused_widget_->removeFocus();
			if (new_focus) {
				focused_widget_ = new_focus;
				focused_widget_->focus();
			}
		}
		return;
	}

	if (focused_widget_)
		focused_widget_->handleKeyDown( key, modifiers );
}

void ceph::GuiWidgets::handleKeyUp(ceph::KeyCode key, ceph::KeyModifiers modifiers)
{
	if (focused_widget_)
		focused_widget_->handleKeyUp(key, modifiers);
}

ceph::GuiWidgets::GuiWidgets(ceph::Scene& scene) :
	parent_(scene), 
	initial_widget_(nullptr), 
	focused_widget_(nullptr)
{
	scene.keyEvent.connect(*this, &ceph::GuiWidgets::handleKeyEvent);	
}

ceph::GuiWidget* ceph::GuiWidgets::getFocus()
{
	return focused_widget_;
}

const ceph::GuiWidget* ceph::GuiWidgets::getFocus() const
{
	return focused_widget_;
}

void ceph::GuiWidgets::setFocus(GuiWidget & widget)
{
	focused_widget_ = &widget;
}

void ceph::GuiWidgets::clearFocus()
{
	focused_widget_ = nullptr;
}
