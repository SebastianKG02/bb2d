#pragma once

#ifndef BB2D_UTILS_JSON_FILE_H
#define BB2D_UTILS_JSON_FILE_H

#include <string>
#include <json.hpp>
#include "EngineSettings.h"

//Define number of spaces to indent JSON
#define BB2D_JSON_OUTPUT_INDENT 4

using namespace nlohmann;

namespace bb2d {

	namespace utils {

		//The default BB2D header generated in new BB2D JSON files
		const json BB2D_DEFAULT_JSON_HEADER =
		{
			{
				"bb2d",
				{
					{
						"version",
						{
							{
								"major", BB2D_VERSION.first
							},
							{
								"minor", BB2D_VERSION.second
							}
						}
					},
					{
						"type", "NONE"
					}
				}
			}
		};

		//A class representing a JSON file within the BB2D engine
		class JSONFile
		{
		public:
			//Emptu constructor
			JSONFile();
			//Create data storage and load JSON file from path
			JSONFile(std::string path);
			//Save current JSON data in <path>
			void saveFile(std::string path="");
			//Load JSON data from <path> and if <makeIfNotFound> create a JSON file in <path>
			bool loadFile(std::string path="", bool makeIfNotFound=false);
			//Reload file from current path
			bool reload();
			//Get raw JSON data pointer
			json* getJSON();
		private:
			json _data;
			std::string _origin = "";
		};
	}
}
#endif
