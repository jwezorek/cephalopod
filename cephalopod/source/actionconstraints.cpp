#include "..\include\cephalopod\actionconstraints.hpp"
#include "..\include\cephalopod\game.hpp"
#include "util.hpp"

void ceph::WrapTorroidally::apply(ceph::Actor& actor, const ActorState& prevState) const
{
	auto& game = ceph::Game::getInstance();
	auto screen_rect = game.getScreenRect();
	auto scr_coord_bounds = game.convertToScreenCoords(actor.getGlobalBounds());

	ceph::Vec2D<float> direction = actor.getPosition() - prevState.getTranslation();

	if (!screen_rect.intersects(scr_coord_bounds)) {
		auto new_bounds = scr_coord_bounds;
		if (new_bounds.x2() < 0.0f && direction.x < 0)
			new_bounds.x = screen_rect.wd;
		else if (new_bounds.x > screen_rect.wd && direction.x > 0)
			new_bounds.x = -scr_coord_bounds.wd;
		if (new_bounds.y2() < 0.0f && direction.y < 0)
			new_bounds.y = screen_rect.hgt;
		else if (new_bounds.y > screen_rect.hgt && direction.y > 0)
			new_bounds.y = -scr_coord_bounds.hgt;
		auto new_position = game.convertFromScreenCoords(ceph::lerpPtInRect(actor.getAnchorPt(), new_bounds));

		actor.setGlobalPosition(new_position);
	}
}
