#pragma once

#ifndef BB2D_ENGINE_ENTITY_H
#define BB2D_ENGINE_ENTITY_H

#include <string>
#include <json.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "JSONFile.h"

using namespace nlohmann;

namespace bb2d {

	namespace engine {

		namespace entity {

			/*
			BB2D customisable type - loaded as raw JSON from disk and can describe plenty of entities.
			Most will be described in entity.json under "templates", as the "Entity" entity is one that is persistent and defined engine wide (as "real" templates are)
			*/
			class Entity
			{
			public:
				//Default constructor, requires a reference (a type) and a path (to load relevant object data)
				Entity(std::string reference, std::string path);
				//Virtual destructor for memory safety
				virtual ~Entity();
				//Initalise this entity (override by child classes)
				virtual void init();
				//Update this entity (override by child classes)
				virtual void update(sf::RenderWindow* w);
				//Provide this eneity with the latest SFML event
				virtual void input(sf::Event* e);
				//Get reference type (such as "sprite_plane_1"
				std::string* getReference();
				//Get path of loaded object
				std::string* getPath();
				//Get type (should be a parent type such as "tex")
				std::string* getType();
				//Get raw udnerlying json object
				json* getDataJSON();
				//Get BB2D Json file wrapper object
				utils::JSONFile* getDataFile();
			protected:
				//BB2D JSON file wrapper object containing raw JSON with additional flavour
				utils::JSONFile* _data;
				//Path to JSON file
				std::string _path;
				//Entity type
				std::string _type;
			};
		}
	}
}
#endif