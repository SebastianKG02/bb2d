#include "JSONFile.h"

using namespace nlohmann;
using namespace bb2d;
using namespace bb2d::utils;

#include <iostream>
#include <fstream>

//Method to load JSON file from <path>, makeIfNotFound will create empty BB2D JSON file if file could not be read
bool JSONFile::loadFile(std::string path, bool makeIfNotFound) {
	//If no path provided, simply re-load current file from <_origin>
	if (path == "") {
		if (_origin == "") {
			_origin = path;
		}
		else {
			path = _origin;
		}
	}

	//Actually read file from path>
	std::ifstream read(path);
	//If successfully open file
	if (read.is_open()) {
		//Dump all contents into _data
		read >> _data;
		//Close file
		read.close();
		//Notify success
		return true;
	}
	//If file could not be open
	else {
		//If file not found
		if (makeIfNotFound) {
			//Open file
			std::ofstream out(path);
			//Apply default header to data store
			_data = BB2D_DEFAULT_JSON_HEADER;
			//Write to file
			out << std::setw(BB2D_JSON_OUTPUT_INDENT) << _data << std::endl;
			//Close file
			out.close();
			//Notify success
			return true;
		}
		else {
			//Notify failure
			return false;
		}
	}
}

JSONFile::JSONFile(std::string path) {
	_origin = path;
}

JSONFile::JSONFile() {

}

void JSONFile::saveFile(std::string path) {
	if (path == "") {
		std::ofstream out(_origin);
		if (out.is_open()) {
			_data["bb2d"]["version"]["major"] = BB2D_VERSION.first;
			_data["bb2d"]["version"]["minor"] = BB2D_VERSION.second;
			out << std::setw(BB2D_JSON_OUTPUT_INDENT) << _data << std::endl;
			out.close();
		}
	}
	else {
		std::ofstream out(path);
		if (out.is_open()) {
			_data["bb2d"]["version"]["major"] = BB2D_VERSION.first;
			_data["bb2d"]["version"]["minor"] = BB2D_VERSION.second;
			out << std::setw(BB2D_JSON_OUTPUT_INDENT) << _data << std::endl;
			out.close();
		}
	}
}

bool JSONFile::reload() {
	return loadFile(_origin);
}

json* JSONFile::getJSON() {
	return &_data;
}