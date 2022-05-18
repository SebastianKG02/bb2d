#pragma once

#include "EngineTypes.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "Resolution.h"
#include "ConfigFile.h"
#include "DebugTypes.h"
#include "Logger.h"
namespace bb2d {

	#ifndef BB2D_GAME_CONTEXT_H
	#define BB2D_GAME_CONTEXT_H
	/*
	* Governing class over the BB2D engine, instanciates SceneManager and AssetManager
	* Also holds information on the current configuration of the engine (such as resolution, debug information)
	*/
	class GameContext
	{
	public:
		//Default constructor (proper token setup, 1080p resolution)
		GameContext();
		//Default destructor (proper sub-member deletion, token clearing, flag flipping)
		~GameContext();
		//Begins initalisation of AssetManager and then SceneManager
		virtual void init();
		//Updates internal game loop
		virtual void update();
		//Polls update
		virtual void input();
		//Draws current scene in SceneManager in window
		virtual void draw();
		sf::RenderWindow* getWindow();
		//Custom resolution input constructor
		GameContext(int screenX, int screenY);
		//Custom resolution and title constructor
		GameContext(int screenX, int screenY, std::string title);
		//Custom resolution, title and token constructor
		GameContext(int screenX, int screenY, std::string title, std::string configPath, GameSettings* gs = nullptr, debug::DebugSettings* ds = nullptr);
		GameSettings* getSettings();
		debug::DebugSettings* getDebugOptions();
		AssetManager* getAssetM();
		SceneManager* getSceneM();
	protected:
		sf::RenderWindow* window;
		GConfigFile config;
		GameSettings* s_game;
		sf::Clock timer;
		float lastFrame;
		float lastSecondFrame;
		float lastGlobalFrame;
		float timePerFrame;
		float targetFPS = 60.0f;
		std::string title;
		int currentFrames = 0;
		int currentGlobalFrames = 0;
		float lastFPS = 0.0f;
		float lastGlobalFPS = 0.0f;
		debug::Logger *_logger;
		debug::DebugSettings* s_debug;
		AssetManager* m_asset;
		SceneManager* m_scene;
	};
	#endif
}
