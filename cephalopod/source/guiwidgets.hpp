#pragma once

#include <vector>

namespace ceph
{
	class Scene;
	class GuiWidget;

	class GuiWidgets
	{
	private:
		Scene& parent_;
		GuiWidget* initial_widget_;
		GuiWidget* focused_widget_;
		std::vector<GuiWidget*> widgets_;

	public:
		GuiWidgets(Scene& scene);
		GuiWidget* getFocus();

		void addWidget(GuiWidget& widget);
		void removeWidget(GuiWidget& widget);
	};

};