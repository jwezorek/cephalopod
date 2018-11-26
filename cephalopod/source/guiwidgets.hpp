#pragma once

#include <vector>
#include "../include/cephalopod/events.hpp"
#include "../include/cephalopod/signals.hpp"

namespace ceph
{
	class Scene;
	class GuiWidget;

	class GuiWidgets : Slot<GuiWidgets>
	{
	private:
		Scene& parent_;
		GuiWidget* initial_widget_;
		GuiWidget* focused_widget_;
		std::vector<GuiWidget*> widgets_;

		void handleKeyDown(ceph::KeyCode key, ceph::KeyModifiers modifiers);
		void handleKeyUp(ceph::KeyCode key, ceph::KeyModifiers modifiers);

	public:
		GuiWidgets(Scene& scene);
		GuiWidget* getFocus();
		const GuiWidget* getFocus() const;

		void setFocus(GuiWidget& widget);
		void clearFocus();
		void addWidget(GuiWidget& widget);
		void removeWidget(GuiWidget& widget);
		void handleKeyEvent(bool isPressed, ceph::KeyCode key, ceph::KeyModifiers modifiers);
	};

};