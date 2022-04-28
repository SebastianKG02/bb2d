#pragma once

#ifndef BB2D_CORE_SPRITESHEET_H
#define BB2D_COTE_SPRITESHEET_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "JSONFile.h"
#include "Logger.h"

namespace bb2d {

	namespace core {

		/*
		* A practical representation of a strip (or grid) of sprites within a bigger sprite.
		* Holds references to textures (as loaded by the AssetManager) as well as sheet proportions.
		* 
		* A sprite sheet has a parent texture (which describes the spritesheet as a whole). It loads
		* individual sprites according to frame size (in x, y) from the parent. Direction is either left/right and
		* up/down. Negative X means each row will be read from right to left. Positive X means each row will be read
		* from left to right. (i.e. descending and ascending X values, the same applies for Y as positive UP and negative
		* DOWN). 
		*/
		class Spritesheet
		{
		public:
			Spritesheet() {

			}
			//Load spritesheet <id> from <path>
			Spritesheet(std::string id, std::string path, debug::Logger* logger) {
				_logger = logger;
				origin = path;
				//Register this spritesheet with the logger
				_logger->registerClass(this, "ANIM/" + id);
				_logger->info(this, "Begin loading animation <" + id + "> from @" + path);
				loadSheetFromJSON(path);
			}

			/*
			* Simple desctructor to clear up all local variables created
			*/
			virtual ~Spritesheet() {
				//Check if can clear variables & void pointers (delete where appropriate)
				if (sheetSize != nullptr) {
					delete sheetSize;
				}

				if (startPos != nullptr) {
					delete startPos;
				}

				if (frameSize != nullptr) {
					delete frameSize;
				}

				if (direction != nullptr) {
					delete direction;
				}

				if (endPos != nullptr) {
					delete endPos;
				}

				if (fps != nullptr) {
					delete fps;
				}
				previousFrame = nullptr;
				currentFrame = nullptr;
				nextFrame = nullptr;
				parentTexture = nullptr;
				//Cleanse sheet
				clearSheet(sheet.size() - 1);
				sheet.clear();
			}

			//Method to load SpriteSheet from JSON
			bool loadSheetFromJSON(const std::string& path) {
				//Load file
				_file = utils::JSONFile(path);
				_file.loadFile();
				auto filePtr = _file.getJSON();
				//If JSON file at <path> is not of correct type
				if ((*filePtr).at("bb2d").at("type").get<std::string>() != "ASSET_SPR_SHEET") {
					//Inform of error
					_logger->error(this, "TYPE MISMATCH!!! EXPECTED ASSET_SPR_SHEET BUT GOT <" + (*filePtr).at("bb2d").at("type").get<std::string>() + std::string("> INSTEAD!!"));
					return true;
				}
				else {
					//If valid type, load values required for simple initalisation
					sheetSize->x = filePtr->at("data").at("size").at("width");
					sheetSize->y = filePtr->at("data").at("size").at("height");
					startPos->x = filePtr->at("data").at("start").at("x");
					startPos->y = filePtr->at("data").at("start").at("y");
					endPos->x = filePtr->at("data").at("end").at("x");
					endPos->y = filePtr->at("data").at("end").at("y");
					frameSize->x = filePtr->at("data").at("frameSize").at("width");
					frameSize->y = filePtr->at("data").at("frameSize").at("height");
					direction->x = filePtr->at("data").at("direction").at("x");
					direction->y = filePtr->at("data").at("direction").at("y");
					parentTextureID = filePtr->at("data").at("texture");
					absolutePath = filePtr->at("data").at("absolutePath");
					sheetID = filePtr->at("data").at("id");
					fps = new int(filePtr->at("data").at("fps"));
					return true;
				}
				return false;
			}


			//Method to save current object representation into a JSON format on the disk at <path>
			//If path is left empty (or "" by default), the current file's original location will be used (overrite)
			bool saveSheetToJSON(std::string path = "") {
				if (path == "") {
					path = origin;
				}
				//Get pointer to raw json data
				auto filePtr = _file.getJSON();
				//Poll current data of object & copy into JSON file
				filePtr->at("data").at("size").at("width") = sheetSize->x;
				filePtr->at("data").at("size").at("height") = sheetSize->y;
				filePtr->at("data").at("start").at("x") = startPos->x;
				filePtr->at("data").at("start").at("y") = startPos->y;
				filePtr->at("data").at("end").at("x") = endPos->x;
				filePtr->at("data").at("end").at("y") = endPos->y;
				filePtr->at("data").at("frameSize").at("width") = frameSize->x;
				filePtr->at("data").at("frameSize").at("height") = frameSize->y;
				filePtr->at("data").at("direction").at("x") = frameSize->x;
				filePtr->at("data").at("direction").at("y") = frameSize->y;
				filePtr->at("data").at("texture") = parentTextureID;
				filePtr->at("data").at("absolutePath") = absolutePath;
				filePtr->at("data").at("sheetID") = sheetID;
				filePtr->at("data").at("faps") = *fps;
				//Actually save the object
				_file.saveFile(path);
			}

			//Get frame <x,y> texture pointer
			const virtual sf::Texture* getFrame(int x, int y) {
				try {
					return sheet.at(getFrameID(x, y));
				}
				catch (std::exception e) {
					return nullptr;
				}
			}

			//Set frame <x,y> texture via pointer
			const virtual bool setFrame(int x, int y, sf::Texture* newFrame) {
				int _pos = getFrameID(x, y);
				_logger->info(this, "Trying to load texture <" + std::to_string(x) + "," + std::to_string(y) + ">... pos:" + std::to_string(_pos));
				try {
					if (&sheet[_pos] != nullptr) {
						sheet.at(_pos) = newFrame;
						return true;
					}
					else {
						sheet.insert({ _pos, newFrame });
						return true;
					}
				}
				catch (std::exception e) {
					_logger->error(this, "Can't set frame <" + std::to_string(x) + "," + std::to_string(y) + ">: " + e.what());
					return false;
				}
			}

			//Get current sheet width (in frames)
			const virtual int getSheetWidth() {
				return sheetSize->x;
			}

			//Get current sheet height (in frames)
			const virtual int getSheetHeight() {
				return sheetSize->y;
			}

			//Get current X component of the location of the first frame in the sheet
			const virtual int getSheetStartX() {
				return startPos->x;
			}

			//Get current Y component of the location of the first frame in the sheet
			const virtual int getSheetStartY() {
				return startPos->y;
			}

			//Get current Width (in pixels) of each frame loaded in
			const virtual int getFrameWidth() {
				return frameSize->x;
			}

			//Get current Height (in pixels) of each frame loaded in
			const virtual int getFrameHeight() {
				return frameSize->y;
			}

			//Get current X component of direction
			const virtual int getDirectionX() {
				return direction->x;
			}

			//Get current Y component of direction
			const virtual int getDirectionY() {
				return direction->y;
			}

			//Get the size of the sheet in (columns, rows)
			const virtual sf::Vector2i* getSheetSize() {
				return sheetSize;
			}

			//Get location of first frame within the sheet
			const virtual sf::Vector2i* getSheetStart() {
				return startPos;
			}

			//Get current frame size of frames loaded in
			const virtual sf::Vector2i* getFrameSize() {
				return frameSize;
			}

			//Get current direction configuration
			const virtual sf::Vector2i* getDirection() {
				return direction;
			}

			//Get reference to previous frame texture
			virtual sf::Texture* getPreviousFrame() {
				return previousFrame;
			}

			//Get reference to current frame texture
			virtual sf::Texture* getCurrentFrame() {
				return currentFrame;
			}

			//Get reference to next frame texture
			virtual sf::Texture* getNextFrame() {
				return nextFrame;
			}
		
			const virtual sf::Texture* getParentTexture() {
				return parentTexture;
			}

			const virtual void setParentTexture(sf::Texture* newParent) {
				parentTexture = newParent;
			}

			const virtual void setParentTextureID(std::string id) {
				parentTextureID = id;
			}

			const virtual std::string getParentTextureID() {
				return parentTextureID;
			}

			const virtual std::string getAbsolutePath() {
				return absolutePath;
			}

			const virtual int getNextFrameID() {
				//If current frame X less than Row Width, go in the +<direction.x>
				frameIDs[0][0] = frameIDs[1][0];
				frameIDs[0][1] = frameIDs[1][1];
				if (frameIDs[1][0] < sheetSize->x && frameIDs[1][1] <= sheetSize->y) {
					frameIDs[2][0] = frameIDs[1][0] + direction->x;
					frameIDs[2][1] = frameIDs[1][1];
				}else if (frameIDs[1][0] == sheetSize->x && frameIDs[1][1] <= sheetSize->y) {
					frameIDs[2][0] = startPos->x;
					frameIDs[2][1] = frameIDs[1][1] + direction->y;
				}
				else if ((frameIDs[1][0] == sheetSize->x && frameIDs[1][1] == sheetSize->y) || (frameIDs[1][0] >= endPos->x && frameIDs[1][1] >= endPos->y)) {
					frameIDs[2][0] = startPos->x;
					frameIDs[2][1] = startPos->y;
				}

				auto result = getFrameID(frameIDs[2][0], frameIDs[2][1]);
				
				if (result >= sheet.size()) {
					frameIDs[2][0] = startPos->x;
					frameIDs[2][1] = startPos->y;
					return getFrameID(frameIDs[2][0], frameIDs[2][1]);
				}
				else {
					return result;
				}
			}

			int getFrameIntID(int a, int b) {
				return frameIDs[a][b];
			}

			virtual sf::Texture* getCurrentTexture() {
				return sheet.at(getFrameID(frameIDs[1][0], frameIDs[1][1]));
			}

			virtual sf::Texture* getPreviousTexture() {
				return sheet.at(getFrameID(frameIDs[0][0], frameIDs[0][1]));
			}

			virtual sf::Texture* getNextTexture() {
				return sheet.at(getFrameID(frameIDs[2][0], frameIDs[2][1]));
			}

			/// <summary>
			/// Update animation logic & frames
			/// </summary>
			/// <returns></returns>
			const virtual void update() {

			}

			// Get  current animation frame in realtime on the GPU
			const virtual sf::Sprite* draw(sf::RenderWindow* w) {
				return nullptr;
			}

			//Get act
			virtual sf::Sprite* getSprite() {
				return nullptr;
			}

			const virtual int next() {
				frameIDs[0][0] = frameIDs[1][0];
				frameIDs[0][1] = frameIDs[1][1];
				frameIDs[1][0] = frameIDs[2][0];
				frameIDs[1][1] = frameIDs[2][1];
				auto result = getNextFrameID();
				return result;
			}

			const virtual int prev() {
				return -1;
			}

			std::map<int, sf::Texture*>* getSheet() {
				return &sheet;
			}
			//Pointers to next lively frames
			sf::Texture* previousFrame = nullptr, * currentFrame = nullptr, * nextFrame = nullptr;
		protected:
			//Simple recursive sheet deletion mechanism
			void clearSheet(int pos) {
				//Control condition
				if (sheet.size() > 0) {
					//Only delete if possible
					if (sheet.at(pos) != nullptr) {
						sheet.at(pos) = nullptr;
					}
					//Recursive call
					clearSheet(--pos);
				}
			}

			//Simple get for frames
			int getFrameID(int x, int y) {
				return ((y * sheetSize->x) + x);
			}

			//Actual JSON file to be loaded
			utils::JSONFile _file;
			//Logger for debug purposes
			debug::Logger* _logger;
			//Actual sheet of sprites
			std::map<int, sf::Texture*> sheet = std::map<int, sf::Texture*>();

			//Actual sheet IDs for each frame
			int frameIDs[3][2] = { {0, 0}, {0, 0}, {0, 0} };
			//Actual texture that was sliced into this sheet
			sf::Texture* parentTexture = nullptr;
			//Reference ID for parent texture
			std::string parentTextureID = "";
			//Absolute path of this Spritesheet
			std::string absolutePath;
			//SpriteSheet identifier
			std::string sheetID;
			//Original location sheet was read from
			std::string origin;
			//Target frames per game second (animation speed)
			int* fps = new int(0);
			//Sheet properties: 
			//		Size (width & height of sheet, in terms of columns & rows)
			//		StartPos (first frame of animation)
			//		EndPos (last frame of animation)
			//		Direction (which way to go in loaded texture (X first then Y))
			sf::Vector2i *sheetSize = new sf::Vector2i(0, 0), 
						 *startPos = new sf::Vector2i(0, 0), 
						 *endPos = new sf::Vector2i(0, 0), 
						 *frameSize = new sf::Vector2i(0, 0),
						 *direction = new sf::Vector2i(0, 0);
		};

	}
}
#endif