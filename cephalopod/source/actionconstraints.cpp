#include "../include/cephalopod/actionconstraints.hpp"
#include "../include/cephalopod/game.hpp"
#include "util.hpp"
#include "actorstate.hpp"

ceph::WrapTorroidally::WrapTorroidally(float horz_padding, float vert_padding) :
	horz_padding_(horz_padding),
	vert_padding_(vert_padding)
{
}

void ceph::WrapTorroidally::apply(ceph::ActorState& state) const
{
	auto& game = ceph::Game::getInstance();
	auto screen_rect = game.getScreenRect();
	screen_rect.inflate(horz_padding_, vert_padding_);
	auto scr_coord_position = game.convertToScreenCoords( ceph::SfPtToCephPt( state.getGlobalPosition()) );

	if (!screen_rect.contains(scr_coord_position)) {
		if (scr_coord_position.x < screen_rect.x)
			scr_coord_position.x += screen_rect.wd;
		else if (scr_coord_position.x > screen_rect.x2())
			scr_coord_position.x -= screen_rect.wd;

		if (scr_coord_position.y < 0.0f)
			scr_coord_position.y += screen_rect.hgt;
		else if (scr_coord_position.y > screen_rect.y2())
			scr_coord_position.y -= screen_rect.hgt;

		auto new_position = game.convertFromScreenCoords(scr_coord_position);
		state.setGlobalPosition(ceph::CephPtToSfPt(new_position) );
	}
}
