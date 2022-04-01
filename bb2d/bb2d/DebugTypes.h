#pragma once
#include <SFML/Graphics/Color.hpp>

#ifndef BB2D_DEBUG_TYPES_H
#define BB2D_DEBUG_TYPES_H

namespace bb2d {

	namespace debug {

		namespace level {
			/*
			* Reference enum for identifying debug level
			*/
			enum DebugLevel
			{
				NONE,
				MINIMAL,
				NORMAL,
				FULL
			};
		}

		/*
		* Container for setting debug options
		*/
		struct DebugSettings
		{
			bool DEBUG_ENABLED = false;
			bool DEBUG_HITBOX_SHOW = false;
			bool DEBUG_CONSOLE = true;
			float DEBUG_HITBOX_LINE_THICKNESS = 1.0f;
			sf::Color DEBUG_HITBOX_LINE_COLOUR = sf::Color::Red;
			level::DebugLevel DEBUG_LEVEL = level::NONE;
		};
	}

}

#endif