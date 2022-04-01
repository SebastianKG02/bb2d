#pragma once

#ifndef BB2D_LOGGER_H
#define BB2D_LOGGER_H	

//Define usage of debug console rule
#define BB2D_LOGGER_DO_DEBUG_CONSOLE true

//Define usage of log file rule
#define BB2D_LOGGER_DO_LOG_FILE true

#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include "Version.h"

namespace bb2d {

	namespace debug {

		/*
		* Preferred logging method for BB2D. Outputs to debug console as well as log file.
		* Handles formatting via LogFormat strings.
		*/
		class Logger
		{
		public:
			//Default constructor
			Logger(char start = '[', char end = ']') {
				//Set reference tags
				tagStart = start;
				tagEnd = end;
				//Init array to store tags for logging outuput & also init map to store ids against pointer
				map = new std::vector<std::pair<void*, std::string*>*>();
				ids = new std::map<void*, int>();
				//Get start of logging session
				_start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

#if BB2D_LOGGER_DO_LOG_FILE == true
				//Get reference path to output file
				path = new std::string("log/");
				(*path) = (*path) + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
				(*path) = (*path) + ".bb2dlog";

				//Create file
				std::ofstream output;
				output.open(*path, std::ostream::out | std::ostream::in | std::ostream::app);
				output << ' ';
				output.flush();
				output.close();
#endif
				//Register this class within logging system
				registerClass(this, "LOGGER");
				info(this, "Logger init complete! Running BB2D V" + std::to_string(BB2D_VERSION.first) + "." + std::to_string(BB2D_VERSION.second));
			}

			//Default destructor
			~Logger() {
				//Loop through contents of class and delete pointers & erase other data where possible
				/*
				do {
					map->at(map->size() - 1)->second->clear();
					delete map->at(map->size() - 1);
					map->erase(map->begin() + (map->size() - 1), map->begin() + (map->size() - 1));
				} while (map->size() > 0);
				*/
				delete map;
				ids->clear();
				delete ids;
				delete path;
			}

			//Register a class to the Logging system with a unique tag and a pointer reference to the class
			bool registerClass(void* object, const std::string& tag = "NO TAG") {
				try {
					//Reserve space for new pair
					map->push_back(new std::pair<void*, std::string*>);
					//Set pair attributes
					map->at(map->size() - 1)->first = object;
					map->at(map->size() - 1)->second = new std::string(tagStart + tag + tagEnd);
					//Get id and register against pointer
					ids->insert({ object, map->size() - 1 });
					//Return success
					return true;
				}
				catch (std::exception e) {
					error(this, "Error registering class with tag <" + tag + ">, Detail: " + e.what());
					return false;
				}
			}

			//Default method for printing to debug console & log file
			bool print(void* object, const std::string& val = "TESTING", const std::string& tag = "INFO") {
				try {
					//Get current time
					auto _t = std::chrono::system_clock::now();
					//Get difference between start time and now
					std::chrono::duration<double> diff = _t - std::chrono::system_clock::from_time_t(_start);

#if BB2D_LOGGER_DO_DEBUG_CONSOLE == true
					//Output to debug console
					std::cout << tagStart << diff.count() << tagEnd << tagStart << tag << tagEnd << tagStart << *map->at(ids->at(object))->second << tagEnd << val << "\n";
#endif

#if BB2D_LOGGER_DO_LOG_FILE == true 
					//Open log file in append mode
					std::ofstream output;
					output.open(*path, std::ios::app);
					//Output to log file
					output << tagStart << diff.count() << tagEnd << tagStart << tag << tagEnd << tagStart << *map->at(ids->at(object))->second << tagEnd << val << "\n";
					output.flush();
					output.close();
#endif
					//Notify success
					return true;
				}
				catch (std::exception e) {
					//notify failure
					std::cout << "!!ERROR!!\t" << e.what() << std::endl;
					return false;
				}
			}

			//Ease of access method to print info message to debug console & log file
			bool info(void* object, const std::string& val = "TESTING") {
				return print(object, val, "INFO");
			}

			//Ease of access method to print warning message to debug console & log file
			bool warn(void* object, const std::string& val = "TESTING") {
				return print(object, val, "WARN!");
			}

			//Ease of access method to print error message to debug console & log file
			bool error(void* object, const std::string& val = "TESTING") {
				return print(object, val, "!!ERR!!");
			}

		private:
			char tagStart = '[';
			char tagEnd = ']';
			std::vector<std::pair<void*, std::string*>*>* map;
			std::map<void*, int>* ids;
			std::string* path;
			std::time_t _start;
		};

	}
}
#endif