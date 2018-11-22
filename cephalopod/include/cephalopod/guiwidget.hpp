#pragma once

#include "actor.hpp"
#include "events.hpp"
#include <string>

namespace ceph
{

	class GuiWidget 
	{
	protected:
		std::string id_;
		bool is_enabled_;
		Actor& self_;

		virtual void onStateChange() = 0;

	public:
		GuiWidget(const std::string& id, Actor& self);

		std::string getId() const;
		bool isEnabled() const;
		void Enable();
		void Disable();

		bool hasFocus() const;
		void SetFocus();

		virtual void handleKeyboardInput(KeyCode key, KeyModifiers mods) = 0;
	};
};