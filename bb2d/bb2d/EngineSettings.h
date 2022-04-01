#pragma once

#ifndef BB2D_ENGINE_SETTINGS_H
#define BB2D_ENGINE_SETTINGS_H

#include "Utils.h"
#include "Version.h"
#include <SFML/Graphics.hpp>

namespace bb2d
{
	namespace engine {

		//Method intended for referencing purposes only
		extern inline std::string getVersion() {
			return BB2D_VERSION.first + "." + BB2D_VERSION.second;
		}

		//Add manual entry to <target> settings
		extern inline void addEntryGameSettings(GameSettings* target, std::string key, std::string val, SettingType type = SettingType::STRING) {
			switch (type) {
			case SettingType::KEY:
				target->keyboardSettings->insert({ key, new std::pair< sf::Keyboard::Key, sf::Keyboard::Key>() });
				target->keyboardSettings->at(key)->first = bb2d::Utils::charToKey(val);
				target->keyboardSettings->at(key)->second = bb2d::Utils::charToKey(val);
				break;
			case SettingType::INT:
				target->integerSettings->insert({ key, new std::pair< uint16_t,uint16_t>() });
				target->integerSettings->at(key)->first = std::stoi(val);
				target->integerSettings->at(key)->second = std::stoi(val);
				break;
			case SettingType::BOOL:
				target->booleanSettings->insert({ key, new std::pair<bool,bool>() });
				target->booleanSettings->at(key)->first = Utils::stringToBool(val);
				target->booleanSettings->at(key)->second = Utils::stringToBool(val);
				break;
			case SettingType::STRING:
				target->stringSettings->insert({ key, new std::pair<std::string, std::string>() });
				target->stringSettings->at(key)->first = val;
				target->stringSettings->at(key)->second = val;
				break;
			}
		}

	}
};

#endif