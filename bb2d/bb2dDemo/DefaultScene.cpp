#include "DefaultScene.h"

DefaultScene::DefaultScene() {
	this->sceneID = 0;
	this->name = "TESTING";
}

void DefaultScene::init() {
	
	int resY = _settings->resolution->Y();
	int resX = _settings->resolution->X();
	std::cout << ref_m_asset->getTexture("desert_tile");
	int sprX = ref_m_asset->getTexture("desert_tile")->getSize().x;
	int sprY = ref_m_asset->getTexture("desert_tile")->getSize().y;

	int _last = 0;
	for (int y = 0; y < resY / sprY; y++) {
		
		for (int x = 0; x < resX / sprX; x++) {
			if (_last == 0) {
				sprites.push_back(new sf::Sprite(*ref_m_asset->getTexture("desert_tile")));
				sprites[sprites.size() - 1]->setPosition(x * sprX, y * sprY);
				_last = 1;
			}
			else {
				_last = 0;
			}
		}
	}
}

void DefaultScene::update(sf::RenderWindow* w) {

}

void DefaultScene::input(sf::Event* e) {

}

void DefaultScene::cleanup() {

}