#pragma once

#ifndef BB2D_RESOLUTION_H
#define BB2D_RESOLUTION_H

#include <string>

namespace bb2d {
	//Simple class to contain resolution data and a quick-fire name reference
	class Resolution
	{
	public:
		//Simple Constructor
		Resolution(int screenX = 0, int screenY = 0);
		//String-based constructor
		Resolution(std::string res);
		//Destructor
		~Resolution();
		//Get for X (width)
		int X();
		//Get for Y (height)
		int Y();
		//Update both X and Y
		void update(int screenX, int screenY);
		void update(std::string res);
		//Update only X
		void updateX(int screenX);
		//Update only Y
		void updateY(int screenY);
		//Get difference between default resolution X and target resolution X (used for UI paddding updates)
		float diffX();
		//Get difference between default resolution Y and target resolution Y (used for UI paddding updates)
		float diffY();
		//Get for resolution reference name
		std::string name();
	private:
		//Screen width (cannot be negative as window would not init)
		unsigned short screen_X;
		//Screen height
		unsigned short screen_Y;
		//Resolution reference name
		std::string resName;
		//Function to convert both X&Y into string and concat, updating resName
		void updateName();
	};
}
#endif