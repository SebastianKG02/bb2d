#pragma once

#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <map>
#include <string>
#include <iostream>
#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <exception>
#include "ConfigFile.h"
#include "JSONFile.h"
#include "Logger.h"
#include "Spritesheet.h"
#include "AssetManager.h"

namespace bb2d {      

	/*
	* Class responsible for the reading of resources from the disk to tbe used at a later date
	*/
	class AssetManager
	{
	public:
		//Default constructor - calls init() method
		AssetManager(debug::Logger *logger);
		//Default destructor - calls cleanup() method
		~AssetManager();
		//Initalises essential variables and loads assets from asset.json
		void init();
		//Frees up all loaded resources from memory
		void cleanup();
		//Gets a texture that has been loaded
		sf::Texture* getTexture(std::string textureName);
		//Gets a soundBuffer that has been loaded
		sf::SoundBuffer* getSound(std::string soundName);
		//Gets a font that has been loaded
		sf::Font* getFont(std::string fontName);
		//Gets a music file that has been identified on the disk
		sf::Music* getMusic(std::string musicName);
		//Gets a spritesheet file that contains loaded animations
		core::Spritesheet* getSpriteSheet(std::string sheetName);
		//Check internal init completion flag
		bool isInitComplete();
		//Check internal cleanup completion flag
		bool isCleanupComplete();
		//Loads a asset manifest file (such as tex.json) which describes resources of <type> to be loaded from disk
		void loadAssetJSON(const std::string& type);
		void loadTextures();
		void loadFonts();
		void loadMusic();
		void loadSounds();
		void loadAnimations();
	private:
		//Store of JSON files loaded during intialisation - is cleared when initialisation is complete
		std::map<std::string, bb2d::utils::JSONFile*> assets;
		//Store of loaded textures
		std::map<std::string, sf::Texture*> tex;
		//Store of loaded sounds
		std::map<std::string, sf::SoundBuffer*> sound;
		//Store of loaded fonts
		std::map<std::string, sf::Font*> fonts;
		//Store of loaded music
		std::map<std::string, sf::Music*> music;
		//Store of loaded Spritesheets
		std::map<std::string, core::Spritesheet*> anim;
		//Initalisation flag
		bool hasFinishedInit = false;
		//Cleanup flag
		bool hasFinishedCleanup = false;
		//Keeps track of created loading threads
		std::vector<int> threadStatus;
		debug::Logger *_logger;
	};
}

#endif