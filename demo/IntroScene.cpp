#include "IntroScene.hpp"
#include "AsteroidsScene.hpp"
#include "cephalopod/game.hpp"
#include "cephalopod/signals.hpp"
#include "cephalopod/scenetransition.hpp"
#include "cephalopod/font.hpp"
#include "cephalopod/fontsheet.hpp"
#include "cephalopod/label.hpp"
#include "cephalopod/button.hpp"

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
		fsheet, "roboto-large", "Cephalopod Obligatory Asteroids Demo", ceph::Label::Justification::Left, ceph::ColorRGB(0,0,0)
	);
	lbl->moveTo( 430, 800 );
	addActor(lbl);

	sprite_sheet_ = ceph::SpriteSheet::create(
		".\\data\\btns.png",
		".\\data\\btns.json"
	);

	auto& key_evt = ceph::Game::getInstance().keyEvent;
	connect(key_evt, &IntroScene::handleKey);

	start_btn_ = ceph::Actor::create<ceph::Button>(
		sprite_sheet_, "start", "start_selected", "start_clicked", "start_disabled"
	);
	addActor(start_btn_);
	start_btn_->moveTo(600, 700);

	exit_btn_ = ceph::Actor::create<ceph::Button>(
		sprite_sheet_, "exit", "exit_selected", "exit_clicked", "exit_disabled"
	);
	addActor(exit_btn_);
	exit_btn_->moveTo(600, 600);
}

void IntroScene::handleKey(bool is_key_down, ceph::KeyCode key, ceph::KeyModifiers modifiers)
{
	if (is_key_down && key == ceph::KeyCode::G)
		ceph::Game::getInstance().setScene<ceph::CrossFadeTransition>( "game", 3.0f );
}
