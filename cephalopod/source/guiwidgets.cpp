#include "guiwidgets.hpp"
#include "../include/cephalopod/scene.hpp"
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

ceph::GuiWidgets::GuiWidgets(Scene& scene) :
	parent_(scene), 
	initial_widget_(nullptr), 
	focused_widget_(nullptr)
{
	
}

ceph::GuiWidget* ceph::GuiWidgets::getFocus()
{
	return focused_widget_;
}
