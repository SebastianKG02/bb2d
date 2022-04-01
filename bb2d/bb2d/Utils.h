#pragma once

#ifndef BB2D_UTILS_H
#define BB2D_UTILS_H

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include "EngineTypes.h"

namespace bb2d {
	namespace Utils {
		/*
		UTILS - A simple class to handle a few universal functions
		V0.0.a-1 Added to project
		- added loadConfigFile (method for loading files)
		- added saveToConfigFile (method to save file)
		*/
		extern inline bool loadConfigFile(std::string file, std::map<std::string, std::string>* target, std::string delim = "=");
		extern inline bool saveToConfigFile(std::string file, std::map<std::string, std::string>* data, std::string delim = "=");
		extern inline std::string boolToString(bool b);
		extern inline bool stringToBool(std::string s);
		extern inline std::string keyToStr(sf::Keyboard::Key key);
		extern inline sf::Keyboard::Key  charToKey(std::string keyt);
		extern inline std::string* strToUpper(std::string* str);
		extern inline std::string* strToLower(std::string* str);
		extern inline std::pair<std::string, std::string> splitString(std::string str, std::string delim);
	}
}
#endif 