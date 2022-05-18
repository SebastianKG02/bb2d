#pragma once

#ifndef BB2D_UI_ELEMENT_H
#define BB2D_UI_ELEMENT_H

#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Math.h"
#include "UITypes.h"
#include <thread>

using namespace bb2d::math;

namespace bb2d {

	namespace ui {

		/*
		Underling base type for all UI elements - self-contained sprite with multiple states
		*/
		class UIElement
		{
		public:
			//Functionality --------------------------------------------------------------------- Functionality
			
			//Default simple constructor - initalises values as needed
			UIElement(vec2 pos, vec2 org, vec2 size, debug::Logger* log = nullptr, const std::string& name = "UIElement");
			//Initalisation (called on Canvas build)
			virtual void init();
			//Empty method - used to provide functionality to UI Element
			virtual void update(sf::RenderWindow* win);
			//Simple draw method
			void draw(sf::RenderWindow* w);
			//Cleans up UI element and deletes all used variables
			void cleanup();
			//Simple destructor to clean up used assets (calls cleanup())
			virtual ~UIElement();
			//Element name getter
			std::string getName();
			//Element name setter
			void setName(const std::string& newname);
			//Functionality --------------------------------------------------------------------- Functionality

			//Object Control ------------------------------------------------------------------- Object Control
			
			//Locks button (will not update)
			void lock();
			//Unlocks button (resumes updates)
			void unlock();
			//Pauses updates for (time)
			void pause(float time);
			//Returns current activity state of UI Element
			bool isActive();
			//Set wether UI element is active to <state>
			void setActive(bool state);
			//Object Control ------------------------------------------------------------------- Object Control

			//State Control --------------------------------------------------------------------- State Control
			
			//Get pointer to state of current object
			UIState* getState();
			//Override current state for this object
			void setState(UIState s);
			//Update texture reference used for state <s> to be the one found at <ref>
			void setStateTex(UIState s, std::string ref);
			//State Control --------------------------------------------------------------------- State Control

			//UICluster Manipulation --------------------------------------------------- UICluster Maniuplation
			
			//Get UICluster (store of elements)
			UICluster* getCluster();
			//Get individual elements by raw ID
			sf::Sprite* getSpriteByID(int id);
			sf::Text* getTextByID(int id);
			sf::Shape* getShapeByID(int id);
			//Get individual elements by identifier
			sf::Sprite* getSpriteByName(const std::string& name);
			sf::Text* getTextByName(const std::string& name);
			sf::Shape* getShapeByName(const std::string& name);
			//Add Component methods
			bool addSprite(const std::string& name, sf::Sprite* what, bool center = false);
			bool addText(const std::string& name, sf::Text* what, bool center = false);
			bool addShape(const std::string& name, sf::Shape* what, bool center = false);
			//UICluster Manipulation --------------------------------------------------- UICluster Maniuplation

			//UIElement Manipulation --------------------------------------------------- UIElement Maniuplation

			void move(vec2 vec);
			void setPosition(vec2 pos);
			vec2* getPosition();
			vec2* getOrigin();
			vec2* getSize();
			void reCenter();
			//UIElement Manipulation --------------------------------------------------- UIElement Maniuplation

			//Composite Manipulation --------------------------------------------------- Composite Manipulation

			void centerSprite(const std::string& name);
			void centerText(const std::string& name);
			void centerShape(const std::string& name);
			void centerSprite(sf::Sprite* obj);
			void centerText(sf::Text* obj);
			void centerShape(sf::Shape* obj);
			//Composite Manipulation --------------------------------------------------- Composite Manipulation

			//THREADING ----------------------------------------------------------------------------- THREADING
			
			//Unlocks this UI element after <time> has passed
			void countdownTimer(float time);
			//THREADING ----------------------------------------------------------------------------- THREADING
		protected:
			//Store of elements used by this UIElement
			UICluster* element;
			//Dimensional definition of this UIElement
			UIRect* rect;
			//Map of states against texture references for AssetManager
			std::map<UIState, std::string> states;
			//Pointer to currently used AssetManager
			AssetManager* ref_m_asset;
			//Current state of object
			UIState currentState;
			//Flag for current lock state
			bool locked = false;
			//Flag for current pause state
			bool paused = false;
			//Store of time 
			float counter = 0;
			//Actual timer used to control pausing states
			sf::Clock internalClock;
			//Pointer to current BB2D logger instance
			debug::Logger* logger;
			//Controls update status (wether shown or not)
			bool active = true;
			//ID reference within Scene
			std::string name;
		private:
			//Raw method that handles retrival by raw numeric ID
			void* getRawElementByID(int id, UIObject type);
			//Raw method that handles retrival by human-friendly string AND uses raw numeric ID to provide output
			void* getRawElementByName(const std::string& name, UIObject type);
			//Raw method that handles adding components (in their proper way) to the UI Cluster
			bool addComponent(const std::string& name, void* obj, UIObject type, bool center = false);
			//Raw method that handles deleting components (in their proper way) from the UI Cluster
			bool delComponent(const std::string& name, UIObject type);
			//Raw method that centers a component based on width and height
			void centerComponent(const std::string& name, UIObject type);
			void centerComponent(void* obj, UIObject type);
			
		};
	}
}
#endif