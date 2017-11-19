#pragma once

#include <memory>
#include "SFML/Graphics.hpp"
#include "../include/cephalopod/texture.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/scene.hpp"
#include "../include/cephalopod/game.hpp"

namespace ceph
{
	class SceneImpl
	{
	public:
		sf::Sprite bkgd_;
		std::shared_ptr<Texture> bkgd_tex_;
		sf::Transform bkgd_mat_;
		ceph::CoordinateMapping coordinate_mapping_mode_;

		SceneImpl();
		void drawBackground(const DrawingContext& rt);
		void setBackground(const std::shared_ptr<Texture>& tex, ceph::CoordinateMapping mapping);
	};
}
