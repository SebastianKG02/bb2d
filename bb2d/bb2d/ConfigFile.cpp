#include "ConfigFile.h"
#include "Utils.h"
#include "Utils.cpp"

//Preferred constructor, using file to load data
ConfigFile::ConfigFile(std::string pathn, std::string delim) {
	Utils::loadConfigFile(pathn.c_str(), &data, delim);
}

//Default constructor with empty map
ConfigFile::ConfigFile() {
	data = std::map<std::string, std::string>();
}

//Get value from map using string key
std::string ConfigFile::getValue(std::string key) {
	try {
		return data.at(key);
	}
	catch (std::exception e) {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
		std::cout << "[DEBUG]Could not get value <" << key << "> from map. " << e.what() << std::endl;
		return "";
#endif
#endif
	}
}

//Add value (val) to map under key (key)
bool ConfigFile::addValue(std::string key, std::string val) {
	data.insert({ key, val });

	//See if successful, return true/false based on result
	if (data.at(key) == val) {
		return true;
	}
	else {
		return false;
	}
}

//Delete value from map, return true to confirm deletion has been successful
bool ConfigFile::delValue(std::string key) {
	data.erase(key);
	return true;
}

//Get pointer to entire map
std::map<std::string, std::string>* ConfigFile::getData() {
	return &data;
}

//Load config file using Utils method
bool ConfigFile::loadConfigFile(std::string file, std::string delim) {
	std::cout << file << std::endl;
	/*if (&data == nullptr) {
		data = std::map<std::string, std::string>();
	}*/
	Utils::loadConfigFile(file, &data, delim);
	return true;
}

//Set value in map (checks for existing values and uses doOverride to see if user wants to override existing data
bool ConfigFile::setValue(std::string key, std::string val, bool doOverride) {
	//If data is present and override is true, overwrite data and return true
	if (data.at(key).length() > 0 && doOverride == true) {
		data[key] = val;
		return true;
	}
	//If data is present and override is false, do nothing and return false
	else if (data.at(key).length() > 0 && doOverride == false) {
		return false;
	}
	//If no data present BUT key is present, set value and return true
	else if (data.at(key).length() <= 0) {
		data[key] = val;
		return true;
	}
	else {
		//If no data and no key present, add data to map and return true
		data.insert({ key, val });
		return true;
	}
}

//Default constructor
GConfigFile::GConfigFile() {
	gs = new GameSettings();
}

//Check if line counts as valid header
bool GConfigFile::headerCheck(std::string line) {
	//BB2D files should always
	if (line[0] == '#') {
		if (*Utils::strToUpper(&line) == "@BB2D#" + engine::getVersion() + "@BB2D#") {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void GConfigFile::loadFromFile(std::string path, GameSettings* target, std::string delim) {
	this->delim = delim;
	//Temporary map used for loading values
	std::map<std::string, std::string> tempMap;
	Utils::loadConfigFile(path, &tempMap);
	gs = target;

	//If file successfully opened, load data into tempMap and return to caller
	if (!tempMap.empty()) {
		//Loop through each line and extract data
		for (auto line: tempMap ) {
			//First part of GConfig dataline should be [T:default] where T is SettingType char (k, i, b, s), default being replaced by the default value set 
			sflag id_split = Utils::splitString(line.first, "]");
			//Get key and value (1 / 2)
			sflag value_split = Utils::splitString(id_split.second, "=");
			//Check if format is valid as per [T:default]
			if (id_split.first.c_str()[0] == '[') {
				//Check if format is valid as per [T:default]
				if (id_split.first.c_str()[2] == ':') {
					//Decide what value needs to be assigned to via the second char in the line
					switch (std::tolower(id_split.first.c_str()[1])) {
					case 'k':
						//Check if key found in config is a valid sfml key
						if (Utils::charToKey(value_split.second) != sf::Keyboard::Key::Unknown) {
							//If so, load into copied gameSettings
							target->keyboardSettings->at(value_split.first)->second = Utils::charToKey(value_split.second);
						}
						else {
							//If not, load default key from copied gameSettings
							target->keyboardSettings->at(value_split.first)->second = target->keyboardSettings->at(value_split.first)->first;
						}
						break;
					case 'i':
						//Check if integer properly casts from string
						if (std::stoi(value_split.second) != NULL) {
							//Load cast value into copied gameSettings
							target->integerSettings->at(value_split.first)->second = std::stoi(value_split.second);
						}
						else {
							//Load default value
							target->integerSettings->at(value_split.first)->second = target->keyboardSettings->at(value_split.first)->first;
						}
						break;
					case 'b':
						//Normalise string then compare to literal and decide whether value is true/false/invalid
						if (*Utils::strToUpper(&value_split.second) == "TRUE") {
							//Where input matches with TRUE, the appropriate flag is flipped
							target->booleanSettings->at(value_split.first)->second = true;
						}
						else if (*Utils::strToUpper(&value_split.second) == "FALSE") {
							//Where input matches with FALSE, the appropriate flag is flipped
							target->booleanSettings->at(value_split.first)->second = false;
						}
						else {
							//In cases where the input is neither TRUE or FALSE, the default value is assigned
							target->booleanSettings->at(value_split.first)->second = target->booleanSettings->at(value_split.first)->first;
						}
						break;
					case 's':
						//Check if string is at least one character in length
						if (value_split.second.size() > 0) {
							//If so, this value is loaded into the copied gameSettings
							target->stringSettings->at(value_split.first)->second = value_split.second;
						}
						else {
							//If not, the default value is used
							target->stringSettings->at(value_split.first)->second = target->stringSettings->at(value_split.first)->second;
						}
						break;
					default:
						//In case there is an invalid character, it is treated the same as string so as to not upset the runtime
						if (value_split.second.size() > 0) {
							target->stringSettings->at(value_split.first)->second = value_split.second;
						}
						else {
							target->stringSettings->at(value_split.first)->second = target->stringSettings->at(value_split.first)->second;
						}
						break;
					}
				}
				else {
					continue;
				}
			}
			else {
				continue;
			}

#ifdef DEBUG_ENABLED
			//ONLY IF DEBUG IS ENABLED!!
			//Detail everything loaded into the map
			std::cout << "[DEBUG]Map loading K:" << id_split.first << " V:" << value_split.second << std::endl;
#endif
		}
	}
	else {
		//if file not loaded successfully, return blank map
#ifdef DEBUG_ENABLED
		std::cerr << "Could not open file @" << path << " please try again." << std::endl;
#endif
	}
}

void GConfigFile::saveToFile(std::string path, GameSettings* source, std::string delim) {
	//Check if source is NULLPTR - if so, use local copy instead
	if (source != nullptr) {
		std::string outString;

		//File to write to
		std::fstream file_o;
		//Open file for writing
		file_o.open(path);

		//If file sucessfully opened
		if (file_o.is_open()) {
			file_o.clear();

			//Dump loaded string settings into file
			for (auto& k : *source->stringSettings) {
				outString.clear();
				//Format output string and put data in place
				outString = '[' + 's' + ':' + k.second->first.c_str() + ']' + *k.first.c_str() + delim + k.second->second + '\n';
				file_o.write(outString.c_str(), outString.size());
			}

			//Dump loaded keyboard settings into file
			for (auto& k : *source->keyboardSettings) {
				outString.clear();
				//Format output string and put data in place
				outString = '[' + 'k' + ':' + Utils::keyToStr(k.second->first).c_str() + ']' + *k.first.c_str() + delim + Utils::keyToStr(k.second->second) + '\n';;
				file_o.write(outString.c_str(), outString.size());
			}

			//Dump loaded boolean settings into file
			for (auto& k : *source->booleanSettings) {
				outString.clear();
				//Format output string and put data in place
				outString = '[' + 'b' + ':' + Utils::boolToString(k.second->first).c_str() + ']' + *k.first.c_str() + delim + Utils::boolToString(k.second->second) + '\n';;
				file_o.write(outString.c_str(), outString.size());
			}

			//Dump loaded integer settings into file
			for (auto& k : *source->integerSettings) {
				outString.clear();
				//Format output string and put data in place
				outString = '[' + 'i' + ':' + std::to_string(k.second->first).c_str() + ']' + *k.first.c_str() + delim + std::to_string(k.second->second).c_str() + '\n';
			}

			file_o.flush();
			file_o.close();
		}
		else {
			//File failed to load
		}
	}
	else {

	}
}

GameSettings* GConfigFile::getSettings() {
	return gs;
}

GConfigFile::GConfigFile(std::string path, GameSettings* source, std::string delim) {
	loadFromFile(path, source, delim);
}