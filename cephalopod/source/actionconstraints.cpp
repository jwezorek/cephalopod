#include "../include/cephalopod/actionconstraints.hpp"
#include "../include/cephalopod/actor.hpp"
#include "../include/cephalopod/game.hpp"
#include "util.hpp"

ceph::WrapTorroidally::WrapTorroidally(float horz_padding, float vert_padding) :
	horz_padding_(horz_padding),
	vert_padding_(vert_padding)
{
}

void ceph::WrapTorroidally::apply(ceph::Actor& actor) const
{
	auto& game = ceph::Game::getInstance();
	auto log_rect = game.getLogicalRect();
	auto log_sz = log_rect.getSize();
	auto pt = actor.getGlobalPosition();

	if (log_rect.contains(pt))
		return;

	float left, right, top, bottom;
	left = log_rect.x;
	right = log_rect.x2();

	if (log_rect.y < log_rect.y2()) {
		top = log_rect.y2();
		bottom = log_rect.y;
	} else {
		top = log_rect.y;
		bottom = log_rect.y2();
	}

	if (pt.x < left)
		pt.x += log_sz.x;
	if (pt.x > right)
		pt.x -= log_sz.x;
	if (pt.y < bottom)
		pt.y += log_sz.y;
	if (pt.y > top)
		pt.y -= log_sz.y;
	
	actor.setGlobalPosition( pt );

	pt = actor.getGlobalPosition();
}
