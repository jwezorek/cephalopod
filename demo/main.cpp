
#include <memory>
#include <Windows.h>
#include "cephalopod/game.hpp"
#include "AsteroidsScene.hpp"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	auto& game = ceph::Game::getInstance();

	game.initialize(ceph::ScreenMode::WindowedWithTitleBar, 1200, 600, std::string("foobar") );
	//game.initialize(ceph::ScreenMode::FullScreenExclusive);
	game.setLogicalCoordinates(
		ceph::CoordinateMapping::UseBlackBars, 
		ceph::Size<float>(400,400)
	);

	auto scene = std::static_pointer_cast<ceph::Scene>(std::make_shared<Asteroids>());
	game.run(scene);

	return 0;
}