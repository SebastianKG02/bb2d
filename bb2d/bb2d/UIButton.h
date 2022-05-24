#pragma once

#ifndef BB2D_UI_BUTTON_H
#define BB2D_UI_BUTTON_H

#include "UIElement.h"
#include <string>
#include "GameContext.h"

using namespace bb2d::math;

namespace bb2d{

    namespace ui {

        /*
        * A class describing a button with four configurable states
        */
        class UIButton : public UIElement
        {
        public:
            //Manual constructor
            UIButton(const std::string& name, AssetManager* ref, debug::Logger* log, std::string tex[4] = { &std::to_string(0) }, vec2 pos = vec2(0, 0), vec2 size = vec2(0, 0), vec2 scale = vec2(1, 1)) : UIElement(pos, vec2(0, 0), size, log, name) {
                this->ref_m_asset = ref;
                this->logger = log;
                this->logger->registerClass(this, "UIB");
                this->logger->info(this, "Registering Button UIElement");
                button = new sf::Sprite();
                addSprite(name, button);
                //Debug information about potential states to be made
                for (int i = 0; i < 4;i++) {
                    this->logger->info(this, "State [" + std::to_string(i) + "]: " + tex[i]);
                }
                try {
                    if (ref_m_asset->getTexture(tex[0]) != nullptr) {
                        button->setTexture(*ref_m_asset->getTexture(tex[0]));
                        //this->currentState = UIState::ACTIVE;

                        setStateTex(UIState::ACTIVE, tex[0]);
                        setStateTex(UIState::HOVER, tex[1]);
                        setStateTex(UIState::CLICK, tex[2]);
                        setStateTex(UIState::INACTIVE, tex[3]);
                        centerSprite(button);
                    }
                    else {
                        this->logger->warn(this, "Texture " + tex[0] + " was nullptr!");
                    }
                }
                catch (std::exception e) {
                    this->logger->warn(this, "Error setting up Button UIElement: " + *e.what());
                }
            };
            //More advanced form of update method found in UIElement
            virtual void update(sf::RenderWindow* w);
        private:
            sf::Sprite* button;
            sf::RectangleShape* rect;
            //0 for button, 1 for rect
            int mode = 0;
            int minX = 0;
            int maxX = 0;
            int minY = 0;
            int maxY = 0;
        };
    }
}
#endif