#pragma once
#include <SceneManager.h>
class DefaultScene : public bb2d::Scene
{
public:
	DefaultScene();
	void init();
	void cleanup();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
};

