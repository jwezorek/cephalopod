#include "IntroScene.hpp"
#include "AsteroidsScene.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/signals.hpp"
#include "cephalopod/scenetransition.hpp"
#include "cephalopod/font.hpp"
#include "cephalopod/fontsheet.hpp"
#include "cephalopod/label.hpp"
#include "cephalopod/sprite.hpp"

IntroScene::IntroScene()
{
	setBackgroundColor(ceph::ColorRGB(255, 255, 255));

	// make a label...
	std::shared_ptr<ceph::Font> font_1 = std::make_shared<ceph::Font>(".\\data\\Roboto-Medium.ttf");
	std::shared_ptr<ceph::Font> font_2 = std::make_shared<ceph::Font>(".\\data\\cmunrm.ttf");

	auto fsheet = std::make_shared<ceph::FontSheet>(
		std::vector<ceph::FontSheet::FontItem> {
			ceph::FontSheet::FontItem("roboto-small", font_1, 30),
			ceph::FontSheet::FontItem("roboto-large", font_1, 60),
			ceph::FontSheet::FontItem(font_2, 50)
		}
	);
	auto lbl = ceph::Actor::create<ceph::Label>(
		fsheet, "roboto-large", "Cephalopod Obligatory Demo Asteroids", ceph::Label::Justification::Left, ceph::ColorRGB(0,0,0)
	);
	lbl->moveTo(200, 300);
	addActor(lbl);

	sprite_sheet_ = ceph::SpriteSheet::create(
		".\\data\\btns.png",
		".\\data\\btns.json"
	);

	auto& key_evt = ceph::Game::getInstance().keyEvent;
	connect(key_evt, &IntroScene::handleKey);

	auto test_btn = ceph::Actor::create<ceph::Sprite>(sprite_sheet_, "start");
	addActor(test_btn );
	test_btn->moveTo(500, 500);
}

void IntroScene::handleKey(bool is_key_down, ceph::KeyCode key, unsigned char modifiers)
{
	if (is_key_down && key == ceph::KeyCode::G)
		ceph::Game::getInstance().setScene<ceph::CrossFadeTransition>( "game", 3.0f );
}
