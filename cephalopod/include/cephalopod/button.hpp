#pragma once

#include "guiwidget.hpp"
#include "sprite.hpp"

namespace ceph
{
	class Button : public Sprite, public GuiWidget
	{
	private:
		std::string frames_[4];

	protected:
		Button(const std::string& id, 
			const std::shared_ptr<const SpriteSheet>& sheet,
			const std::string& normal_frame, 
			const std::string& selected_frame, 
			const std::string& clicked_frame,
			const std::string& disabled_ftame
		);

	public:
	};
};