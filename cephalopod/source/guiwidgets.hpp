#pragma once

#include <unordered_map>

namespace ceph
{
	class Scene;
	class GuiWidget;

	class GuiWidgets
	{
	private:
		Scene& parent_;
		std::unordered_map<std::string, GuiWidget&> widgets_;

		void addWidget(GuiWidget& widget);
		void removeWidget(GuiWidget& widget);

	public:
		GuiWidgets(Scene& scene);
		GuiWidget* getFocus();
	};

};