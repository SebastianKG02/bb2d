#include "JSONFile.h"

using namespace nlohmann;
using namespace bb2d;
using namespace bb2d::utils;

#include <iostream>
#include <fstream>

bool JSONFile::loadFile(std::string path, bool makeIfNotFound) {
	if (path == "") {
		path = _origin;
	}
	std::ifstream read(path);
	if (read.is_open()) {
		read >> _data;
		read.close();
		return true;
	}
	else {
		if (makeIfNotFound) {
			std::ofstream out(path);
			_data = BB2D_DEFAULT_JSON_HEADER;
			out << std::setw(BB2D_JSON_OUTPUT_INDENT) << _data << std::endl;
			out.close();
			return true;
		}
		else {
			return false;
		}
	}
}

JSONFile::JSONFile(std::string path) {
	_origin = path;
}

void JSONFile::saveFile(std::string path) {
	if (path == "") {
		std::ofstream out(_origin);
		if (out.is_open()) {
			out << std::setw(BB2D_JSON_OUTPUT_INDENT) << _data << std::endl;
			out.close();
		}
	}
	else {
		std::ofstream out(path);
		if (out.is_open()) {
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