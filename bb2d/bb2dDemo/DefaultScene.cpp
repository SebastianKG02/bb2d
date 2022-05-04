#include "DefaultScene.h"

DefaultScene::DefaultScene() {
	this->sceneID = 0;
	this->name = "TESTING";
}

void DefaultScene::init() {
	
	/*
	int resY = _settings->resolution->Y();
	int resX = _settings->resolution->X();
	std::cout << ref_m_asset->getTexture("desert_tile");
	int sprX = ref_m_asset->getTexture("desert_tile")->getSize().x;
	int sprY = ref_m_asset->getTexture("desert_tile")->getSize().y;
	*/
	anims.push_back(new bb2d::core::Animation(ref_m_asset->getSpriteSheet("ammo-strip"), new sf::Sprite(), 60.0f));
	
	anims[0]->getSprite()->setPosition(300, 300);

	anims.push_back(new bb2d::core::Animation(ref_m_asset->getSpriteSheet("tower-spin"), new sf::Sprite(), 60.0f));
	anims[1]->getSprite()->setScale(5.0f, 5.0f);
	anims[1]->getSprite()->setPosition(325, 600);

	/*
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
	*/
}

void DefaultScene::update(sf::RenderWindow* w) {
	anims[0]->update();
	anims[1]->update();
}

void DefaultScene::input(sf::Event* e) {

}

void DefaultScene::cleanup() {

}