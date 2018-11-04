#include "IntroScene.hpp"
#include "AsteroidsScene.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/signals.hpp"
#include "cephalopod/scenetransition.hpp"
#include "cephalopod/font.hpp"
#include "cephalopod/fontsheet.hpp"
#include "cephalopod/label.hpp"

void IntroScene::setMainScene(const std::shared_ptr<Asteroids>& a)
{
	asteroids_ = a;
}

IntroScene::IntroScene()
{
	std::shared_ptr<ceph::Font> font = std::make_shared<ceph::Font>(".\\data\\cmunrm.ttf");
	auto fsheet = std::make_shared<ceph::FontSheet>(
		std::vector<ceph::FontSheet::FontItem> {
			ceph::FontSheet::FontItem(font, 30),
			ceph::FontSheet::FontItem(font, 60),
		}
	);
	auto lbl = ceph::Actor::create<ceph::Label>(
		fsheet, "cmunrm.ttf", 60, "Doohickey"
	);
	lbl->moveTo(100, 100);

	addActor(lbl);

	setBackgroundColor( ceph::ColorRGB(255, 0, 0) );
	auto bkgd = std::make_shared<ceph::Texture>(".\\data\\snoopy.png");
	setBackground(bkgd);

	auto& key_evt = ceph::Game::getInstance().keyEvent;
	connect(key_evt, &IntroScene::handleKey);
}

void IntroScene::handleKey(bool is_key_down, ceph::KeyCode key, unsigned char modifiers)
{
	if (is_key_down && key == ceph::KeyCode::G)
		ceph::Game::getInstance().setScene<ceph::CrossFadeTransition>( "game", 3.0f );
}