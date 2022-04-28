#pragma once
#include <string>
#include <map>
#include "EngineSettings.h"
#include "EngineTypes.h"

#ifndef BB2D_CONFIG_FILE_H
#define BB2D_CONFIG_FILE_H
#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED
#endif

namespace bb2d {

	/*
	ConfigFile - A component of the Utils class, meant for storing simple data in a form mirroring a ".properties" file
	Stores data as a std::map of string as the key and string as the value
	V0.0.a-1 Added to project
	- inital addition
	*/
	class ConfigFile
	{
	public:
		ConfigFile(std::string pathn, std::string delim = "=");
		ConfigFile();
		std::string getValue(std::string key);
		std::map<std::string, std::string>* getData();
		bool addValue(std::string key, std::string val);
		bool delValue(std::string key);
		bool setValue(std::string key, std::string val, bool doOverride = false);
		bool loadConfigFile(std::string file, std::string delim = "=");
	private:
		std::string path;
		std::string delim = "=";
		std::map<std::string, std::string> data;
	};

	//Alternitive version of config file
	//Supports type
	class GConfigFile
	{
	public:
		GConfigFile();
		GConfigFile(std::string path, GameSettings* source, std::string delim = "=");
		GameSettings* getSettings();
		void loadFromFile(std::string path, GameSettings* target, std::string delim = "=");
		void saveToFile(std::string path, GameSettings* source = nullptr, std::string delim = "=");
	private:
		bool headerCheck(std::string line);
		GameSettings* gs;
		std::string delim;
	};
}

#endif
