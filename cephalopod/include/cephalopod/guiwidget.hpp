#pragma once

#include "events.hpp"
#include <string>

namespace ceph
{
	class GuiWidget 
	{
	protected:
		std::string id_;
		bool isEnabled_;

		virtual void onEnabledStateChange() = 0;
		virtual void onFocusStateChange() = 0;

	public:
		GuiWidget(const std::string& id);

		bool isEnabled() const;
		void Enable();
		void Disable();

		bool hasFocus() const;
		void SetFocus();

		virtual void handleKeyboardInput(KeyCode key, unsigned char mods) = 0;
	};
};