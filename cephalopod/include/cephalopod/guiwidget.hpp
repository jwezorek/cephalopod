#pragma once

#include "actor.hpp"
#include "events.hpp"
#include <string>
#include <memory>

namespace ceph
{

	class GuiWidget 
	{
		friend class GuiWidgets;

	protected:
		bool is_enabled_;
		std::weak_ptr<GuiWidget> next_, prev_;

		virtual void handleKeyDown(ceph::KeyCode key, ceph::KeyModifiers modifiers) = 0;
		virtual void handleKeyUp(ceph::KeyCode key, ceph::KeyModifiers modifiers) = 0;
		virtual void onStateChange() = 0;

		void onWidgetAttachedToScene();
		void onBeforeWidgetDetachedFromScene();
		void setNext(const std::shared_ptr<GuiWidget>& widget);
		void setPrev(const std::shared_ptr<GuiWidget>& widget);
		GuiWidget* getNext();
		GuiWidget* getPrev();

		Actor& self();
		const Actor& self() const;

	public:
		GuiWidget();
		bool isEnabled() const;
		void enable();
		void disable();

		bool hasFocus() const;
		void focus();
		void removeFocus();
		void focusNext();
		void focusPrev();

		template<typename T>
		void setNext(const T& next)
		{
			setNext(
				std::static_pointer_cast<GuiWidget>(next)
			);
		}

		template<typename T>
		void setPrev(const T& prev)
		{
			setPrev(
				std::static_pointer_cast<GuiWidget>(prev)
			);
		}
	};
};