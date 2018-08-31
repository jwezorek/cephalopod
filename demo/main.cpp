
#include <memory>
#include <Windows.h>
#include "cephalopod/game.hpp"
#include "AsteroidsScene.hpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	std::unique_ptr<ceph::Game> game = ceph::Game::createInstance();

	game->keyEvent.connect(*game, 
		[=](bool isPressed, ceph::KeyCode key, unsigned char modifiers) {
			if (isPressed && key == ceph::KeyCode::Escape)
				ceph::Game::getInstance().quit();
		}
	);

	//game->initialize(ceph::WindowMode::WindowedWithTitleBar, 1024, 768);
	//game->setLogicalCoordinates(ceph::CoordinateMapping::UseBlackBars, ceph::Vec2<float>(1024, 768), ceph::CoordinateSystem::LowerLeftOriginAscendingY);

	game->initializeFullscreen( );
	game->setLogicalCoordinates(ceph::CoordinateMapping::UseBlackBars, ceph::Vec2<float>(1920, 1080), ceph::CoordinateSystem::LowerLeftOriginAscendingY);

	auto scene = ceph::Scene::create<Asteroids>();
	game->run(scene);

	return 0;
}