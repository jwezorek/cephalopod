
#include <memory>
#include <Windows.h>
#include "cephalopod/game.hpp"
#include "AsteroidsScene.hpp"
#include "IntroScene.hpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	std::unique_ptr<ceph::Game> game = ceph::Game::createInstance();

	game->keyEvent.connect(*game, 
		[=](bool isPressed, ceph::KeyCode key, unsigned char modifiers) {
			if (isPressed && key == ceph::KeyCode::Escape)
				ceph::Game::getInstance().quit();
		}
	);

	game->initialize(ceph::WindowMode::WindowedWithTitleBar, 1800, 1000);
	game->setLogicalCoordinates(ceph::CoordinateMapping::UseBlackBars, ceph::Vec2<float>(1800, 1000), ceph::CoordinateSystem::LowerLeftOriginAscendingY);

	//game->initializeFullscreen( );
	//game->setLogicalCoordinates(ceph::CoordinateMapping::UseBlackBars, ceph::Vec2<float>(1920, 1080), ceph::CoordinateSystem::LowerLeftOriginAscendingY);

	game->registerScene<IntroScene>( "intro" );
	game->registerScene<Asteroids>( "game", ceph::ColorRGB(10, 30, 60));
	game->run("intro");

	return 0;
}