#pragma once

#ifndef BB2D_SCENE_MANAGER_H
#define BB2D_SCENE_MANAGER_H

#include <vector>
#include <map>
#include <iostream>
#include "Utils.h"
#include "AssetManager.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include "UIElement.h"

namespace bb2d {
	/*
	The Scene class is a template class for all future scenes to be loaded in the SceneManager
	*/
	class Scene
	{
	public:
		Scene(int id, std::string name, GameSettings* settings);
		Scene();  
		virtual void init();
		virtual void cleanup();
		void preInit(AssetManager* ref, GameSettings* settings);
		virtual void draw(sf::RenderWindow* w);
		virtual void update(sf::RenderWindow* w);
		virtual void input(sf::Event* e);
		void lock();
		void unlock();
		bool isActive();
		std::string getFriendlyName();
		int getID();
		void setID(int id);
		void kill();
	protected:
		std::vector<sf::Sprite*> sprites;
		std::vector<sf::Sound*> sounds;
		std::vector<sf::Text*> text;
		std::vector<ui::UIElement*> ui;
		std::vector<sf::Shape*> shapes;
		GameSettings* _settings;
		AssetManager* ref_m_asset;
		int sceneID;
		bool active = false;
		std::string name;
	};

	/*
	V0.1a - Scene Manager
	The SceneManager is responsible for switching between scenes (or screens),
	initalising them and loading all the appropriate resources
	*/
	class SceneManager
	{
	public:
		void init(AssetManager* ref, debug::Logger *logger, GameSettings* settings);
		void cleanup();
		void draw(sf::RenderWindow* w);
		void input(sf::Event* e);
		void update(sf::RenderWindow* w);
		GameSettings* getSettings();
		int getCurrentSceneID();
		Scene* getCurrentScene();
		Scene* getScene(int id);
		Scene* getNext(int id);
		Scene* getPrev(int id);
		void next();
		void previous();
		void set(int id);
		void setNext(int id);
		void setPrev(int id);
		void addScene(Scene* scene);
		void delScene(int id);
		bool isInitComplete();
		bool isCleanUpComplete();
	private:
		AssetManager* ref_m_asset;
		std::map<int, Scene*> scenes;
		debug::Logger* _logger;
		GameSettings* _settings;
		int nextScene;
		int prevScene;
		int currScene;
		bool initComplete;
		bool cleanupComplete;
	};
}

#endif