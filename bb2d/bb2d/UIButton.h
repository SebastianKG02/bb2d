#pragma once

#ifndef BB2D_UI_BUTTON_H
#define BB2D_UI_BUTTON_H

#include "UIElement.h"
#include <string>
#include "GameContext.h"

namespace bb2d{

    namespace ui {

        struct UIButtonConfig 
        {
            float pos[2];
            float scale[2];
            std::string tex;
            AssetManager* am;
        };

        /*
        * A class describing a button with four configurable states
        */
        class UIButton : public UIElement
        {
        public:
            UIButton(float x, float y, std::string defTex, AssetManager* ref, float scale_x = 1.0f, float scale_y = 1.0f);
            UIButton(float pos[2], std::string type, AssetManager* ref, float scale[2]);
            virtual void update(sf::RenderWindow* w);
        private:
            int minX = 0;
            int maxX = 0;
            int minY = 0;
            int maxY = 0;
        };
    }
}
#endif