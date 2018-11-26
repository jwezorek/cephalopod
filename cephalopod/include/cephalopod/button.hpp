#pragma once

#include "guiwidget.hpp"
#include "sprite.hpp"

namespace ceph
{
	class Button : public Sprite, public GuiWidget
	{
		friend class Actor;

	private:
		std::string frames_[4];
		bool is_clicked_;

	protected:
		Button(const std::shared_ptr<SpriteSheet>& sheet,
			const std::string& normal_frame, 
			const std::string& selected_frame, 
			const std::string& clicked_frame,
			const std::string& disabled_ftame
		);

		void handleKeyDown( ceph::KeyCode key, ceph::KeyModifiers modifiers );
		void handleKeyUp( ceph::KeyCode key, ceph::KeyModifiers modifiers );
		void onStateChange();

		void onAttachScene() override;
		void onBeforeDetachScene() override;

	public:

		Signal<Actor&> clickEvent;

	};
};