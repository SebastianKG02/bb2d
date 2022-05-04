#include "GameContext.h"

using namespace bb2d;
using namespace bb2d::engine;
using namespace bb2d::debug;

// GAME CONTEXT DEFINITION ----------------------------------------------------

//Default constructor for GameContext - make new Token and Resolution
GameContext::GameContext() {
	s_game->resolution = new Resolution(1920, 1080);
	window = new sf::RenderWindow(sf::VideoMode(s_game->resolution->X(), s_game->resolution->Y()), "BB2D " + getVersion());
	_logger = new bb2d::debug::Logger();
}

GameContext::GameContext(int screenX, int screenY) {
	s_game->resolution = new Resolution(screenX, screenY);
	window = new sf::RenderWindow(sf::VideoMode(s_game->resolution->X(), s_game->resolution->Y()), "BB2D " + getVersion());
	_logger = new bb2d::debug::Logger();
}

GameContext::GameContext(int screenX, int screenY, std::string title) {
	s_game->resolution = new Resolution(screenX, screenY);
	window = new sf::RenderWindow(sf::VideoMode(s_game->resolution->X(), s_game->resolution->Y()), title);
	_logger = new bb2d::debug::Logger();
}

GameContext::GameContext(int screenX, int screenY, std::string title, std::string configPath, GameSettings* gs, debug::DebugSettings* ds) {
	if (gs == nullptr) {
		s_game = new GameSettings();
		s_game->resolution = new Resolution(screenX, screenY);
	}
	else {
		s_game = gs;
		s_game->resolution = new Resolution(screenX, screenY);
	}

	if (ds == nullptr) {
		s_debug = new DebugSettings();
	}
	config = GConfigFile(configPath, gs);
	window = new sf::RenderWindow(sf::VideoMode(s_game->resolution->X(), s_game->resolution->Y()), title);
	_logger = new bb2d::debug::Logger();
}

//Cleanup method for the engine
GameContext::~GameContext() {
	//Flip cleanup flag
	GLOBAL_BB2D_STATE = bb2ds::CLEANUP_BEGIN;

	if (m_scene != nullptr) {
		m_scene->cleanup();
		delete m_scene;
	}

	if (m_asset != nullptr) {
		m_asset->cleanup();
		delete m_asset;
	}

	if (s_game != nullptr) {
		for (auto& map : *s_game->booleanSettings) {
			delete map.second;
		}
		for (auto& map : *s_game->integerSettings) {
			delete map.second;
		}
		for (auto& map : *s_game->stringSettings) {
			delete map.second;
		}
		for (auto& map : *s_game->keyboardSettings) {
			delete map.second;
		}
	}

	if (s_debug != nullptr) {
		delete s_debug;
	}

	delete _logger;

	//Flip cleanup compelte flag
	GLOBAL_BB2D_STATE = bb2ds::CLEANUP_COMPLETE;
}

GameSettings* GameContext::getSettings() {
	return s_game;
}

DebugSettings* GameContext::getDebugOptions() {
	return s_debug;
}

//Begins initalisation of AssetManager and SceneManager - loads config files as well
void GameContext::init() {
	if (GLOBAL_BB2D_STATE != bb2ds::FAILURE) {
		//Flip initalisation begin flag
		GLOBAL_BB2D_STATE = bb2ds::INIT_BEGIN;

		_logger->registerClass(this, "GameContext");
		_logger->info(this, "Beginning init...");

		if (m_asset == nullptr) {
			m_asset = new AssetManager(_logger);
			//Initalise global asset manager
			_logger->info(this, "GLOBAL ASSET MANAGER INIT COMPLETE");
		}
		if (m_scene == nullptr) {
			m_scene = new SceneManager();
			//Initalise global scene manager
			m_scene->init(m_asset, _logger, s_game);
			_logger->info(this, "GLOBAL SCENE MANAGER INIT COMPLETE");
		}
		//Load game settings

		GLOBAL_BB2D_STATE = bb2ds::INIT_COMPLETE;
	}
}

void GameContext::update() {
	if (m_scene != nullptr) {
		m_scene->update(window);
	}
	else {
		GLOBAL_BB2D_STATE = bb2ds::FAILURE;
	}
}

sf::RenderWindow* GameContext::getWindow() {
	return window;
}

void GameContext::input() {
	sf::Event e;
	while (window->pollEvent(e)) {
		if (e.type == sf::Event::Closed) {
			window->close();
		}
		m_scene->input(&e);
	}
}

AssetManager* GameContext::getAssetM() {
	return m_asset;
}

SceneManager* GameContext::getSceneM() {
	return m_scene;
}

void GameContext::draw() {
	window->clear(sf::Color::White);
	m_scene->draw(window);
	window->display();
}

// GAME CONTEXT DEFINITION END -------------------------------------------------