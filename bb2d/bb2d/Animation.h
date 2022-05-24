#pragma once

#ifndef BB2D_ANIMATION_H
#define BB2D_ANIMATION_H
#include "Spritesheet.h"

namespace bb2d {

	namespace core {

		/*
		Class containing an explictily animated Spritesheet, with its' own Sprite to control		
		*/
		class Animation : public Spritesheet {
		public:
			//Default constructor to null
			Animation() {
				sheet = nullptr;
				fps = 0.0f;
				internalTimer = sf::Clock();
				drawTarget = nullptr;
			}

			//Proper constructor for a functioning animation
			Animation(Spritesheet* source, sf::Sprite* target, float fps){
				drawTarget = target;
				sheet = source;
				this->fps = fps;
				internalTimer = sf::Clock();
				init();
			}

			//Get representation of the current sprite & draw it to the screen
			virtual sf::Sprite* draw(sf::RenderWindow* w) {
				if (drawTarget != nullptr) {
					w->draw(*drawTarget);
					return drawTarget;
				}
				else {
					return nullptr;
				}
			}

			//Get reference to current sprite being used as the canvas
			virtual sf::Sprite* getSprite() {
				return drawTarget;
			}

			//Standard method that progresses this animation basd on time per frame
			const virtual void update() {
				//Get current time of frame
				auto thisFrame = internalTimer.getElapsedTime();
				//std::cout << "[" << timePerFrame << "]" << "LAST: " << lastFrame.asSeconds() << '\t' << "THIS: " << thisFrame.asSeconds() << std::endl;
				//If this frame is after last frame + timePerFrame, get next frame 
				if (thisFrame.asSeconds() > lastFrame.asSeconds() + timePerFrame) {
					try {
						//Get next frame
						doNextFrame();
						//Set last frame to be this frame
						lastFrame = thisFrame;
					}
					catch (std::exception e) {
						//Notify of error
						std::cout << "Error moving to the next Frame :" << e.what() << std::endl;
					}
				}
			}
			void doNextFrame() {
				//Switch from current frame to next frame
				/*
				(*sheet->getPreviousFrame()) = *sheet->getSprite()->getTexture();
				(*sheet->getCurrentFrame()) = *sheet->getSheet()->at(sheet->getNextFrameID());
				(*sheet->getNextFrame()) = *sheet->getCurrentFrame();
				*/
				auto next = sheet->next();
				sheet->currentFrame = sheet->getSheet()->at(next);
				drawTarget->setTexture(*sheet->currentFrame);
			}
		protected:
			const virtual void init() {
				timePerFrame = (fps / sheet->getSheet()->size()) / fps;
			}
			//Go to next frame & set new next frame
			
		private:
			//Actual sheet to be used as the source of all animations parts
			Spritesheet* sheet;
			//Target frames per second for the animation
			float fps;
			//Internal framerate timer
			sf::Clock internalTimer;
			//Actual representation of the target sprite to be drawn to on the screen
			sf::Sprite* drawTarget;
			//Time of last frame
			sf::Time lastFrame;
			//Amount of time allocated to each frame
			float timePerFrame = 0.0f;
		};
	}
}

#endif