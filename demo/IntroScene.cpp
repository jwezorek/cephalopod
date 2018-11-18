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
	std::shared_ptr<ceph::Font> font_1 = std::make_shared<ceph::Font>(".\\data\\Roboto-Medium.ttf");
	std::shared_ptr<ceph::Font> font_2 = std::make_shared<ceph::Font>(".\\data\\cmunrm.ttf");

	auto fsheet = std::make_shared<ceph::FontSheet>(
		std::vector<ceph::FontSheet::FontItem> {
			ceph::FontSheet::FontItem("roboto-small", font_1, 30),
			ceph::FontSheet::FontItem("roboto-large", font_1, 60),
			ceph::FontSheet::FontItem(font_2, 50),
		}
	);
	auto lbl = ceph::Actor::create<ceph::Label>(
		fsheet, "roboto-small", "For the love\nof Snoopy\nyo yo yo", ceph::Label::Justification::Right
	);
	lbl->moveTo(200, 300);

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