#include "../include/cephalopod/actionconstraints.hpp"
#include "../include/cephalopod/game.hpp"
#include "util.hpp"

void ceph::WrapTorroidally::apply(ceph::Actor& actor) const
{
	auto& game = ceph::Game::getInstance();
	auto screen_rect = game.getScreenRect();
	auto scr_coord_bounds = game.convertToScreenCoords(actor.getGlobalBounds());

	if (!screen_rect.intersects(scr_coord_bounds)) {
		auto new_bounds = scr_coord_bounds;
		if (new_bounds.x2() < 0.0f)
			new_bounds.x += screen_rect.wd + scr_coord_bounds.wd;
		else if (new_bounds.x > screen_rect.wd)
			new_bounds.x -= screen_rect.wd + scr_coord_bounds.wd;
		if (new_bounds.y2() < 0.0f)
			new_bounds.y += screen_rect.hgt + scr_coord_bounds.wd;
		else if (new_bounds.y > screen_rect.hgt)
			new_bounds.y -= screen_rect.hgt + scr_coord_bounds.wd;
		auto new_position = game.convertFromScreenCoords(ceph::lerpPtInRect(actor.getAnchorPt(), new_bounds));

		actor.setGlobalPosition(new_position);
	}
}
