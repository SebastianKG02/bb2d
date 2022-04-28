#pragma once
#include "UIElement.h"
#include "UIButton.h"

#ifndef BB2D_UI_TEXT_BUTTON
#define BB2D_UI_TEXT_BUTTON

namespace bb2d {

	namespace ui {

		class UITextButton : public UIButton
		{
		public:
			UITextButton();
			UITextButton(sf::Vector2f pos, std::string defTex, AssetManager* ref, sf::Vector2f scale, sf::Font* textFont, const std::string& text = "TESTING");
			UITextButton(float x, float y, std::string defTex, AssetManager* ref, sf::Font* textFont, const std::string& text = "TESTING", float scale_x = 1.0f, float scale_y = 1.0f);
			UITextButton(float pos[2], std::string type, AssetManager* ref, float scale[2], sf::Font* textFont, const std::string& text = "TESTING");
			void init();
			void update(sf::RenderWindow* win);
			sf::Text* getText();
			sf::Font* getFont();
			std::string* getTextString();
			~UITextButton();
		private:
			sf::Text textElement;
			sf::Font* textFont;
			std::string* text;
		};

	}
}
#endif