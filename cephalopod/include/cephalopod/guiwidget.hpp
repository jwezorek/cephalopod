#pragma once

#include "actor.hpp"
#include "events.hpp"
#include <string>

namespace ceph
{

	class GuiWidget 
	{
	protected:
		bool is_enabled_;
		Actor& self_;

		virtual void handleKeyboardInput(KeyCode key, KeyModifiers mods) = 0;
		virtual void onStateChange() = 0;

	public:
		GuiWidget(Actor& self);
		bool isEnabled() const;
		void Enable();
		void Disable();

		bool hasFocus() const;
		void SetFocus();

		
	};
};