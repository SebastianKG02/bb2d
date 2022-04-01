#pragma once
#include <map>
#include <string>
#include "Resolution.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>

#ifndef BB2D_ENGINE_TYPES
#define BB2D_ENGINE_TYPES

namespace bb2d {

	//Simple enum for holding engine status
	enum BB2DSTATUS
	{
		/*
		Error type 1 (usually also followed by a more specfific number in logging)
		This error type is mainly intended for engine errors, such as critical files not being found
		or resources running out.
		*/
		FAILURE = -1,
		/*
		Error type 2 (usually also followed by a more specific number/descriptor in logging)
		This error type is mainly intended for non-engine errors, such as OpenGL, OpenAL, SFML e.t.c. errors.
		*/
		FAILURE_CRITICAl = -2,
		//The default state of the engine - nothing has been done, but the library has initalised successfully
		IDLE = 0,
		//Engine has begun initalisation - this means that a valid GameContext has been created and it has called its' init() function
		INIT_BEGIN = 1,
		//Engine has completed initalisation - resources have been loaded correctly, and the engine is getting ready to draw.
		INIT_COMPLETE = 2,
		/*
		Engine has ssuccessfully completed all pre - running checks and now can begin drawing
		This flag remains constant unless a failure is thrown, or the engine has to shut down naturally i.e. by the user pressing the exit key
		Even in the event of a failure (as long as it is not a critical failure) cleanup will occur.
		*/
		STANDARD = 3,
		//Cleanup has begun - stop drawing to the screen and begin deleting first Scenes and their members, and then AssetManager assets
		CLEANUP_BEGIN = 4,
		//Cleanup has finished - checking if everything has been correctly deleted
		CLEANUP_COMPLETE = 5,
		//Everything has been correctly deleted and application can now exit.
		EXIT = 6
	};

	//Shorthand reference variant
	typedef BB2DSTATUS bb2ds;

	//Current global BB2D engine status
	//Starts at IDLE, and can only be moved to INIT_BEGIN
	extern bb2ds GLOBAL_BB2D_STATE;

	//A pair of configurable boolean values, used as <default value, user current value> in terms of held data
	typedef std::pair<bool, bool> bflag;
	//A pair of configurable integer values, used as <default value, user current value> in terms of held data
	typedef std::pair<uint16_t, uint16_t> iflag;
	//A pair of configurable SFML Keyboard Key values, used as <default value, user current value> in terms of held data
	typedef std::pair<sf::Keyboard::Key, sf::Keyboard::Key> kflag;
	//A pair of configurable Strings, used as <default value, user current value in terms of held data
	typedef std::pair<std::string, std::string> sflag;

	/*
	* Container for setting game options
	*/
	struct GameSettings
	{
		std::map<std::string, kflag*>* keyboardSettings = new std::map<std::string, kflag*>();
		std::map<std::string, iflag*>* integerSettings = new std::map<std::string, iflag*>();
		std::map<std::string, bflag*>* booleanSettings = new std::map<std::string, bflag*>();
		std::map<std::string, sflag*>* stringSettings = new std::map<std::string, sflag*>();
		Resolution* resolution;
	};

	enum class SettingType
	{
		KEY,
		INT,
		BOOL,
		STRING
	};


}

#endif