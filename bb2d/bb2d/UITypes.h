#pragma once

#ifndef BB2D_UI_TYPES_H
#define BB2D_UI_TYPES_H

#include <vector>
#include <map>
#include "AssetManager.h"
#include "vec.h"

using namespace bb2d::math;

namespace bb2d {

	namespace ui {

		/*
		Current state of this UI element (used as for state checking and functionality)
		*/
		enum class UIState
		{
			INACTIVE,
			LOCK = INACTIVE,
			HOVER,
			SELECT = HOVER,
			ACTIVE,
			CLICK,
			TRUE,
			FALSE
		};

		//Simple reference to type of object UI is handling
		enum class UIObject
		{
			SPR,
			TXT,
			SHP,
			SPRITE = SPR,
			TEXT = TXT,
			SHAPE = SHP
		};

		/*
		A hold for dimensional definitions of a UI element, forming a rectangle
		*/
		struct UIRect
		{
			//Position of rect
			vec2 pos;
			//Origin point of rect
			vec2 origin;
			//Rect size (width, height)
			vec2 size;

			//Default constructor, inintalises all internal vec2s
			UIRect(float posX = 0, float posY = 0, float originX = 0, float originY = 0, float width = 0, float height = 0) {
				pos = vec2(posX, posY);
				origin = vec2(originX, originY);
				size = vec2(width, height);
			}

			//Vec2 based constructor
			UIRect(vec2 pos = vec2(), vec2 origin = vec2(), vec2 size = vec2()) {
				this->pos = pos;
				this->origin = origin;
				this->size = size;
			}
		};

		template <class T>
		struct UIComposite
		{
			//Vector of pointers to type stored by this Composite
			std::vector<T*>* data;
			//Store of human-friendly IDs to map onto vector
			std::map<std::string, int>* id_ref;

			//Default constructor, intialises vector & map
			UIComposite() {
				data = new std::vector<T*>();
				id_ref = new std::map<std::string, int>();
			}

			//Default destructor loops through vector and deletes sprites, clears idref
			~UIComposite() {
				for (int i = 0; i < data->size(); i++) {
					delete data->at(i);
				}
				delete data;

				//Clear reference array
				delete id_ref;
			}
		};

		/*
		Contains pointers to vectors of UI element pointers
		Used to contain everything used by UIElements to draw
		*/
		struct UICluster
		{
			//Sprites used by this UI Cluster
			UIComposite<sf::Sprite>* spr;
			//Text elements used by this UI Cluster
			UIComposite<sf::Text>* txt;
			//Shape elements used by this UI Cluster
			UIComposite<sf::Shape>* shp;

			//Default constructor
			UICluster() {
				spr = new UIComposite<sf::Sprite>();
				txt = new UIComposite<sf::Text>();
				shp = new UIComposite<sf::Shape>();
			}

			//Default destructor
			~UICluster() {
				//Loop through each vector, delete elements as needed and then delete the vector itself
				spr->~UIComposite();
				delete spr;

				txt->~UIComposite();
				delete txt;

				shp->~UIComposite();
				delete shp;
			}
		};

	}

}

#endif