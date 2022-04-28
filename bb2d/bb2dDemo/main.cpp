#pragma once
#include <JSONFile.cpp>
#include <GameContext.h>
#include "DefaultScene.h"

#ifndef BB2D_DEMO
#define BB2D_DEMO

int main() {

	bb2d::GameSettings game_settings;
	bb2d::engine::addEntryGameSettings(&game_settings, std::string("resolution"), std::string("1280x720"));
	bb2d::engine::addEntryGameSettings(&game_settings, "key_escape", bb2d::Utils::keyToStr(sf::Keyboard::Key::Escape), bb2d::SettingType::KEY);
	bb2d::GameContext my_game = bb2d::GameContext(720, 1280, "BB2D WORKS!", "/config.cfg", &game_settings);
	my_game.init();
	DefaultScene ds;
	my_game.getSceneM()->addScene(&ds);
	my_game.getSceneM()->next();
	while (my_game.getWindow()->isOpen()) {
		my_game.input();
		my_game.update();
		my_game.draw();
	}
	//my_game.~GameContext();
}

#endif