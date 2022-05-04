#include "GameContext.h"

using namespace bb2d;
using namespace bb2d::engine;
using namespace bb2d::debug;

// GAME CONTEXT DEFINITION ----------------------------------------------------

//Default constructor for GameContext - make new Token and Resolution
GameContext::GameContext() {
	timePerFrame = 1.0f / targetFPS;
	timer = sf::Clock();
	s_game->resolution = new Resolution(1920, 1080);
	this->title = title;
	window = new sf::RenderWindow(sf::VideoMode(s_game->resolution->X(), s_game->resolution->Y()), "BB2D " + getVersion());
	_logger = new bb2d::debug::Logger();
}

GameContext::GameContext(int screenX, int screenY) {
	timePerFrame = 1.0f / targetFPS;
	timer = sf::Clock();
	s_game->resolution = new Resolution(screenX, screenY);
	this->title = title;
	window = new sf::RenderWindow(sf::VideoMode(s_game->resolution->X(), s_game->resolution->Y()), "BB2D " + getVersion());
	_logger = new bb2d::debug::Logger();
}

GameContext::GameContext(int screenX, int screenY, std::string title) {
	s_game->resolution = new Resolution(screenX, screenY);
	timer = sf::Clock();
	timePerFrame = 1.0f / targetFPS;
	this->title = title;
	window = new sf::RenderWindow(sf::VideoMode(s_game->resolution->X(), s_game->resolution->Y()), title);
	_logger = new bb2d::debug::Logger();
}

GameContext::GameContext(int screenX, int screenY, std::string title, std::string configPath, GameSettings* gs, debug::DebugSettings* ds) {
	timePerFrame = 1.0f / targetFPS;
	timer = sf::Clock();
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
	this->title = title;
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
		//Update global FPS (actual performance indicator, going as fast as possible)
		//If timer reached lastGlobalFrame + 1 second or further
		if (timer.getElapsedTime().asSeconds() >= lastGlobalFrame + 1.0f) {
			//Update last global frame
			lastGlobalFrame = timer.getElapsedTime().asSeconds();
			//Update last global FPS value
			lastGlobalFPS = ++currentGlobalFrames / 1.0f;
			//Reset current global frame counter
			currentGlobalFrames = 0;
		}
		else {
			//If not, increase counter
			currentGlobalFrames++;
		}

		//Logical (Fixed) Update logic (how fast the acutal game runs logically, the higher the "smoother" the experience"
		if (timer.getElapsedTime().asSeconds() >= lastSecondFrame + 1.0f) {
			//Update last fixed frame 
			lastSecondFrame = timer.getElapsedTime().asSeconds();
			//Update last fixed fps value
			lastFPS = ++currentFrames / 1.0f;
			//Reset current fixed frame counter
			currentFrames = 0;
			//If time ahead of last frame & delay, update game logic
		}else if (timer.getElapsedTime().asSeconds() > lastFrame + timePerFrame) {
			m_scene->update(window);
			//Update last frame
			lastFrame = timer.getElapsedTime().asSeconds();
			//update current fixed frame counter
			currentFrames++;
		}

		//Set FPS notification in title bar of BB2D window
		window->setTitle(title + "[Logic FPS: " + std::to_string(lastFPS) + "][Global FPS: " + std::to_string(lastGlobalFPS) + "]");
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