#include "guiwidgets.hpp"

void ceph::GuiWidgets::addWidget(GuiWidget & widget)
{
}

void ceph::GuiWidgets::removeWidget(GuiWidget & widget)
{
}

ceph::GuiWidgets::GuiWidgets(Scene& scene) :
	parent_(scene)
{
}

ceph::GuiWidget* ceph::GuiWidgets::getFocus()
{
	return nullptr;
}
