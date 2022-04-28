#include "SceneManager.h"

using namespace bb2d;
using namespace bb2d::engine;
using namespace bb2d::ui;
using namespace bb2d::Utils;

/*
SCENE DEFINITION
*/

//Default constructor for all scenes 
Scene::Scene(int id, std::string name, GameSettings* settings, debug::Logger* log) {
	logger = log;
	logger->registerClass(this, name);
	logger->info(this, "Initalising scene...");
	this->sceneID = id;
	this->name = name;
	this->active = false;
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	anims = std::vector<core::Animation*>();
	logger->info(this, "Complete");
	_settings = settings;
}

void Scene::lock() {
	if (active == true) {
		active = false;
	}
}

void Scene::unlock() {
	if (active == false) {
		active = true;
	}
}

bool Scene::isActive() {
	return active;
}

//Get localised name
std::string Scene::getFriendlyName() {
	return name;
}

//Simple cleanup method (deletes all maps, clears both ID vars)
void Scene::cleanup() {
	for (auto& sprite : sprites) {
		delete sprite;
	}

	for (auto& textm : text) {
		delete textm;
	}

	for (auto& sound : sounds) {
		delete sound;
	}

	for (auto& anim : anims) {
		delete anim;
	}

	for (auto& ui_e : ui) {
		//ui_e->cleanup();
		delete ui_e;
	}

	if (ref_m_asset != nullptr) {
		delete ref_m_asset;
	}
}

//More destructive cleanup method
void Scene::kill() {
	cleanup();
	name.clear();
	this->sceneID = NULL;
}

Scene::Scene() {
	this->sceneID = NULL;
	this->name = "";
}

void Scene::preInit(AssetManager* ref, GameSettings* settings) {
	this->ref_m_asset = ref;
	this->_settings = settings;
	this->init();
}

void Scene::init() { 

}

//Simple drawing method, have to pass reference to current window for UI functionality
//Added functionality to draw hitboxes
void Scene::draw(sf::RenderWindow* w) { 
	//debug only - set up ghost hitbox
#if DB_SHOW_HITBOXES == true
	sf::RectangleShape rect = sf::RectangleShape();
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(3.f);
#endif

	//Draw sprites
	for (auto sprite : sprites) {
		w->draw(*sprite);
#if DB_SHOW_HITBOXES == true
		//Draw sprite hitbox
		rect.setPosition(sprite->getPosition());
		rect.setSize(sf::Vector2f(sprite->getGlobalBounds().width, sprite->getGlobalBounds().height));
		w->draw(rect);
#endif
	}

	//Draw UI elements
	for (auto ui_e : ui) {
		w->draw(*ui_e->getSprite());
#if DB_SHOW_HITBOXES == true
		//Draw UIE hitbox
		rect.setPosition(ui_e->getSprite()->getPosition());
		rect.setSize(sf::Vector2f(ui_e->getSprite()->getGlobalBounds().width, ui_e->getSprite()->getGlobalBounds().height));
		w->draw(rect);
#endif
	}

	//Draw text
	for (auto textm : text) {
		w->draw(*textm);
#if DB_SHOW_HITBOXES == true
		//Draw text hitboxes
		rect.setPosition(textm->getPosition());
		rect.setSize(sf::Vector2f(textm->getGlobalBounds().width, textm->getGlobalBounds().height));
		w->draw(rect);
#endif
	}

	//Draw animations
	for (auto anim : anims) {
		w->draw(*anim->draw(w));
	}

	//Draw any primitive shapes
	for (auto shape : shapes) {
		w->draw(*shape);
	}
}

void Scene::update(sf::RenderWindow* w) { }
void Scene::input(sf::Event* e) { 
}

//Simple get for scene's ID
int Scene::getID() {
	return sceneID;
}

//Set internal identifier (override)
void Scene::setID(int id) {
	this->sceneID = id;
}

/*
SCENEMANAGER DEFINITION
*/

//Initalise SceneManager, register all scenes
void SceneManager::init(AssetManager* ref, debug::Logger *logger, GameSettings* settings) {
	//Set up SceneManager working variables
	_logger = logger;
	_logger->registerClass(this, "SceneManager");
	_logger->info(this, "Initalising SceneManager...");
	SceneManager::nextScene = 0;
	SceneManager::prevScene = -1;
	SceneManager::currScene = -1;
	SceneManager::scenes = std::map<int, Scene*>();
	SceneManager::initComplete = false;
	SceneManager::cleanupComplete = false;
	ref_m_asset = ref;
	scenes = std::map<int, Scene*>();
	_settings = settings;
	_logger->info(this, "Initalisation complete.");
}

bool SceneManager::isInitComplete() {
	return initComplete;
}

bool SceneManager::isCleanUpComplete() {
	return cleanupComplete;
}

//Clean up all resources used by scenes by calling their member cleanup function
void SceneManager::cleanup() {
	if (initComplete == true && cleanupComplete == false) {
		for (auto& scene : scenes) {
			scene.second->kill();
			delete &scene;
		}
	}
}

//Get current scene ID
int SceneManager::getCurrentSceneID() {
	return SceneManager::currScene;
}

//Get current scene
Scene* SceneManager::getCurrentScene() {
	return scenes.at(currScene);
}

//Get any scene by ID
Scene* SceneManager::getScene(int id) {
	try {
		return scenes.at(id);
	}
	catch (std::exception e) {
		return nullptr;
	}
}

//Switch to next scene
void SceneManager::next() {
	_logger->info(this, "Moving Scene! CURRENT: " + std::to_string(currScene) + " & NEXT: " + std::to_string(nextScene));
	if (prevScene > -1 && (getScene(prevScene) != nullptr)) {
		getScene(prevScene)->cleanup();
	}

	if (currScene > -1) {
		getScene(currScene)->lock();
	}

	prevScene = currScene;
	currScene = nextScene;

	for (;;) {
		if (prevScene > -1 && getScene(prevScene)->isActive() == false) {
			getScene(prevScene)->lock();
			getScene(currScene)->unlock();
			break;
		}
		else if (prevScene < 0) {
			getScene(currScene)->unlock();
			break;
		}
	}

	getScene(currScene)->preInit(ref_m_asset, _settings);
	getScene(currScene)->unlock();
	_logger->info(this, "Move complete successfully.");
}

//Set scene (override of system)
void SceneManager::set(int id) {
	prevScene = currScene;
	currScene = id;
}

//Set next scene (override)
void SceneManager::setNext(int id) {
	nextScene = id;
}

//Set previous scene (override)
void SceneManager::setPrev(int id) {
	prevScene = id;
}

Scene* SceneManager::getNext(int id) {
	return scenes.at(id);
}

Scene* SceneManager::getPrev(int id) {
	return scenes.at(id);
}

void SceneManager::delScene(int id) {
	if (scenes.find(id)->first == id) {
		scenes.erase(id);
	}
}

void SceneManager::addScene(Scene* scene) {
	try {
		scenes.insert({ scene->getID(), scene });
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
		std::cout << "[SM]Added scene " << scene->getID() << " (or '" << scene->getFriendlyName() << "')" << std::endl;
#endif
#endif
	}
	catch (std::exception e) {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
		std::cout << "[SM]Could not scene " << scene->getID() << " (or '" << scene->getFriendlyName() << "')" << std::endl;
		std::cout << "[SM]Detail: " << e.what() << std::endl;
#endif
#endif
	}
}

//Switch to previous scene
void SceneManager::previous() {
	getScene(prevScene)->init();
	int prev = prevScene;
	prevScene = currScene;
	currScene = prev;
}

//Draw current scene
void SceneManager::draw(sf::RenderWindow* w) {
	scenes.at(currScene)->draw(w);
}

//Get input for current scene
void SceneManager::input(sf::Event* e) {
	//std::cout << currScene << std::endl;
	//std::cout << scenes.at(currScene) << std::endl;
	scenes.at(currScene)->input(e);
}

//Update current scene
void SceneManager::update(sf::RenderWindow* w) {
	//std::cout << currScene << std::endl;
	//std::cout << scenes.at(currScene) << std::endl;
	if (scenes.at(currScene)->isActive() == true) {
		scenes.at(currScene)->update(w);
	}
}