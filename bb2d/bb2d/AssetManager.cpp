#include "AssetManager.h"
#include <thread>

using namespace bb2d;

//Check if initalisation is complete
bool AssetManager::isInitComplete() {
	return hasFinishedInit;
}

//Check if cleanup is complete
bool AssetManager::isCleanupComplete() {
	return hasFinishedCleanup;
}

//Method to get loaded texture
sf::Texture* AssetManager::getTexture(std::string textureName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (tex.at(textureName) != nullptr) {
				return tex.at(textureName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e){
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

//Method to get loaded sound
sf::SoundBuffer* AssetManager::getSound(std::string soundName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (sound.at(soundName) != nullptr) {
				return sound.at(soundName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e) {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

//Method to get loaded font
sf::Font* AssetManager::getFont(std::string fontName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (fonts.at(fontName) != nullptr) {
				return fonts.at(fontName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e) {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

//Method to get loaded font
core::Spritesheet* AssetManager::getSpriteSheet(std::string sheetName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (anim.at(sheetName) != nullptr) {
				return anim.at(sheetName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e) {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

//Method to get loaded Music
sf::Music* AssetManager::getMusic(std::string musicName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (music.at(musicName) != nullptr) {
				return music.at(musicName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e) {

		}
	}
	else {
		return nullptr;
	}
}

/*
Method responsible for loading relevant resource JSON manifests from disk
*/
void AssetManager::loadAssetJSON(const std::string& type) {
	//Register thread status identifier
	threadStatus.push_back(0);
	int _id = threadStatus.size() - 1;
	//Create temporary variables
	std::string tempString;
	std::string idString;
	//Get reference JSON of main asset manifest
	auto ptr = assets["main"]->getJSON();

	//Double check is valid asset
	if ((*ptr)["bb2d"]["type"] == "M_ASSET_MAIN") {
		//Check if resource manifest for textures has been found (create as typeName 
		assets.insert({ type, new bb2d::utils::JSONFile((*ptr).at("data").at(type).get<std::string>()) });
		//Check if file sucessfully opens as JSON file
		_logger->info(this, "Now loading <" + type + "> from " + (*ptr).at("data").at(type).get<std::string>());
		if (assets[type]->loadFile((*ptr).at("data").at(type).get<std::string>(), true) == true) {
			//Load relevant file and get JSON
			auto ptr_data = assets[type]->getJSON();
			//Loop through each entry in resource manifest
			for (auto& ref : (*ptr_data)["data"]["list"]) {
				//Create temporary data
				idString = ref;
				tempString = (*ptr_data)["data"][idString];
				//Attempt to load resource based on type parameter
				switch (type.c_str()[0]) {
				case 't':
					//Insert blank texture reference
					tex.insert({ std::string(idString), new sf::Texture() });

					//Try to load resource - if this fails for any reason, nullptr is used
					if (tex[idString]->loadFromFile(tempString) == false) {
						tex[idString] = nullptr;
					}
					break;
				case 's':
					//Insert blank sound reference
					sound.insert({ std::string(idString), new sf::SoundBuffer() });

					//Try to load resource - if this fails for any reason, nullptr is used
					if (sound[idString]->loadFromFile(tempString) == false) {
						sound[idString] = nullptr;
					}

					break;
				case 'm':
					//Insert blank music reference
					music.insert({ std::string(idString), new sf::Music() });

					//Try to load resource - if this fails for any reason, nullptr is used
					if (music[idString]->openFromFile(tempString) == false) {
						music[idString] = nullptr;
					}

					break;
				case 'f':
					//Insert blank font resource
					fonts.insert({ std::string(idString), new sf::Font() });

					//Try to load resource - if this fails for any reason, nullptr is used
					if (fonts[idString]->loadFromFile(tempString) == false) {
						fonts[idString] = nullptr;
					}
				case 'a':
					anim.insert({ std::string(idString), new core::Spritesheet(idString, tempString, _logger) });
					//Set parent texture
					anim[idString]->setParentTexture(getTexture(anim[idString]->getParentTextureID()));
					for (int y = 0; y < anim[idString]->getSheetHeight(); y++) {
						for (int x = 0; x < anim[idString]->getSheetWidth(); x++) {
							//Create ID for this frame <idString_01> e.t.c.
							std::string frameID = std::string(idString + "_" + std::to_string(x + (y * anim[idString]->getSheetWidth())));
							//Reserve space for it in the textures vector
							tex.insert({ frameID, new sf::Texture() });
							//Send debug message
							_logger->info(this, "Loading frame " + std::to_string(x + (y * anim[idString]->getSheetWidth())) + " for <" + idString + ">");
							//Actually load the texture from the parent image <idString>
							tex[frameID]->loadFromFile(anim[idString]->getAbsolutePath(), sf::IntRect(anim[idString]->getSheetWidth() * x, anim[idString]->getSheetHeight() * y, anim[idString]->getFrameWidth(), anim[idString]->getFrameHeight()));
							//anim[idString]->setFrame(x, y, tex[frameID]);
							//Set actual animation frame to be created texture
							_logger->info(this, "        frame " + std::to_string(x + (y * anim[idString]->getSheetWidth())) + " for <" + idString + ">: " + std::to_string(anim[idString]->setFrame(x, y, tex[frameID])));
						}
					}
					//Insert spritesheet & attempt to load

					break;
				default:
					//By default, if an unsupported type is passed to this function, it shouldn't do anything malicious like try to load it anyway
					break;
				}

				_logger->info(this, "Loaded <" + idString + " " + type + "> from " + tempString);
			}

			//Flip threadstatus flag
			threadStatus[_id] = 1;
		}
	}
}

void AssetManager::loadTextures() {
	_logger->info(this, "Starting LoadTexture Thread");
	loadAssetJSON("tex");
	_logger->info(this, "LoadTexture complete.");
}

void AssetManager::loadSounds() {
	_logger->info(this, "Starting LoadSound Thread");
	loadAssetJSON("sound");
	_logger->info(this, "LoadSound complete.");
}

void AssetManager::loadMusic() {
	_logger->info(this, "Starting LoadMusic Thread");
	loadAssetJSON("music");
	_logger->info(this, "LoadMusic complete.");
}

void AssetManager::loadFonts() {
	_logger->info(this, "Starting LoadFonts Thread");
	loadAssetJSON("font");
	_logger->info(this, "LoadFont complete.");
}

void AssetManager::loadAnimations() {
	_logger->info(this, "Starting LoadAnimation Thread");
	loadAssetJSON("anim");
	_logger->info(this, "LoadAnimation complete.");
}

//Initalise AssetManager, load all resources 
void AssetManager::init() {
	_logger->registerClass(this, "AssetManager");
	_logger->info(this, "Beginning AssetManager initalisation");
	//Only load everything once
	//initalise init/clean flags
	AssetManager::hasFinishedInit = false;
	AssetManager::hasFinishedCleanup = false;
	//Initalise empty config
	ConfigFile main_Config;
	//initalise all maps
	AssetManager::tex = std::map<std::string, sf::Texture*>();
	AssetManager::sound = std::map<std::string, sf::SoundBuffer*>();
	AssetManager::fonts = std::map<std::string, sf::Font*>();
	AssetManager::music = std::map<std::string, sf::Music*>();
	AssetManager::anim = std::map<std::string, core::Spritesheet*>();
	assets = std::map<std::string, bb2d::utils::JSONFile*>();
	assets.insert({ "main", new bb2d::utils::JSONFile("asset.json") });

	assets["main"]->loadFile("asset.json", true);
	auto ptr = assets["main"]->getJSON();

	//If file version is set properly, populate file with default values
	//Versions have to match EXACTLY!
	if ((*ptr)["bb2d"]["version"]["major"].get<int>() == BB2D_VERSION.first
		&& (*ptr)["bb2d"]["version"]["minor"].get<int>() == BB2D_VERSION.second) {
		(*ptr)["bb2d"]["type"] = "M_ASSET_MAIN";
		(*ptr)["data"]["tex"] = "res\\tex.json";
		(*ptr)["data"]["sound"] = "res\\sound.json";
		(*ptr)["data"]["music"] = "res\\music.json";
		(*ptr)["data"]["font"] = "res\\font.json";
		(*ptr)["data"]["anim"] = "res\\anim.json";
	}
	else {
		//If a version mismatch has been found (BB2D version > File version or vice versa)
		_logger->error(this, "ASSETMANAGER ASSET.JSON VERSION MISMATCH!");
		_logger->info(this, "Current version: " + std::to_string(BB2D_VERSION.first) + "." + std::to_string(BB2D_VERSION.second));
		std::string _foundVersion = (*ptr)["bb2d"]["version"]["major"].dump(BB2D_JSON_OUTPUT_INDENT);
		_foundVersion += ".";
		_foundVersion += (*ptr)["bb2d"]["version"]["minor"].dump(BB2D_JSON_OUTPUT_INDENT);

		_logger->info(this, "Target version: " + _foundVersion);
		std::exit(-1);
	}

	threadStatus = std::vector<int>();

	//Register relevant threads for resouce loading
	std::thread _texLoader(&AssetManager::loadTextures, this);
	std::thread _soundLoader(&AssetManager::loadSounds, this);
	std::thread _fontLoader(&AssetManager::loadFonts, this);
	std::thread _musicLoader(&AssetManager::loadMusic, this);

	//Begin threads
	_texLoader.join();
	_soundLoader.join();
	_fontLoader.join();
	_musicLoader.join();

	//Pause continuation of program logic until reading threads are finished
	do {
		//Check latest entry for completion
		if (threadStatus[threadStatus.size()-1] == 1) {
			//If completed, erase this threadStatus
			threadStatus.erase(threadStatus.begin() + (threadStatus.size() - 1));
		}
	} while (threadStatus.size() > 0);

	//First phase loading complete
	_logger->info(this, "First phase init complete, begin phase two...");
	std::thread _animLoader(&AssetManager::loadAnimations, this);
	_animLoader.join();

	//Pause continuation of program logic until reading threads are finished
	do {
		//Check latest entry for completion
		if (threadStatus[threadStatus.size() - 1] == 1) {
			//If completed, erase this threadStatus
			threadStatus.erase(threadStatus.begin() + (threadStatus.size() - 1));
		}
	} while (threadStatus.size() > 0);

	assets["main"]->saveFile();
	assets["main"]->reload();
	//Set init to be complete after
	if (isInitComplete() != true) {
		//Only pass forward if asset file is found
		hasFinishedInit = true;
	}
}

//Simple cleanup method
void AssetManager::cleanup() {
	//Only run this method if init is complete and cleanup has not ran
	if (isInitComplete() == true && isCleanupComplete() == false) {
		//Loop through texture map and delete all loaded textures
		tex.clear();

		//Loop through music map and delete all loaded music
		for (auto& res : music) {
			delete res.second;
		}

		//Loop through font map and delete all loaded fonts
		for (auto& res : fonts) {
			delete res.second;
		}
		//Loop through sound map and delete all loaded sounds
		for (auto& res : sound) {
			delete res.second;
		}

		for (auto& asset : assets) {
			asset.second->saveFile();
		}
		assets.clear();

		//Set cleanup flag
		hasFinishedCleanup == true;
	}
}

//Default constructor - calls init method
AssetManager::AssetManager(debug::Logger *logger) {
	_logger = logger;
	init();
}

//Default desctrutor - calls cleanup method
AssetManager::~AssetManager() {
	cleanup();
}