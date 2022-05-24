#include "UIButton.h"

using namespace bb2d;
using namespace bb2d::ui;

//Update state based on mouse position
void UIButton::update(sf::RenderWindow* w) {
	if (isActive()) {
		//Re-update bounds per frame
		minX = button->getPosition().x - (button->getGlobalBounds().width / 2);
		maxX = button->getPosition().x + (button->getGlobalBounds().width / 2);
		minY = button->getPosition().y - (button->getGlobalBounds().height / 2);
		maxY = button->getPosition().y + (button->getGlobalBounds().height / 2);

		//Poll relative mouse position in window
		float mouse_x = sf::Mouse::getPosition(*w).x;
		float mouse_y = sf::Mouse::getPosition(*w).y;

		//Only update if not locked
		if (locked == false) {
			currentState = UIState::ACTIVE;
			//Check if mouse is within texture rect
			if (((mouse_x >= minX) && (mouse_x <= maxX)) && ((mouse_y >= minY) && (mouse_y <= maxY))) {
				//If true set to hover state
				currentState = UIState::HOVER;
				//If user is hoveing over button AND presses lclick
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					//Report state as being clicked
					currentState = UIState::CLICK;
				}
			}
		}
		else {
			//If locked, do not process any input
			currentState = UIState::LOCK;
		}

		//Switch texture based on state
		switch (currentState)
		{
		case UIState::HOVER:
			button->setTexture(*ref_m_asset->getTexture(states.at(UIState::HOVER)));
			break;
		case UIState::INACTIVE:
			button->setTexture(*ref_m_asset->getTexture(states.at(UIState::INACTIVE)));
			break;
		case UIState::CLICK:
			button->setTexture(*ref_m_asset->getTexture(states.at(UIState::CLICK)));
			break;
		case UIState::ACTIVE:
			button->setTexture(*ref_m_asset->getTexture(states.at(UIState::ACTIVE)));
			break;
		default:
			button->setTexture(*ref_m_asset->getTexture(states.at(UIState::ACTIVE)));
			break;
		}
	}
}