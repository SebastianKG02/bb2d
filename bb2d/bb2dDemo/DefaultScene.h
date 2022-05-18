#pragma once

#ifndef BB2D_DEMO_DEFAULT
#define BB2D_DEMO_DEFAULT
#include <SceneManager.h>
#include <UIButton.h>

class DefaultScene : public bb2d::Scene
{
public:
	DefaultScene() {

	}
	DefaultScene(int id, std::string name, bb2d::GameSettings* settings) : bb2d::Scene(id, name, settings) {

	}
	void init();
	void cleanup();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
};
#endif