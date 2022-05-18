#include "UIElement.h"

using namespace bb2d;
using namespace bb2d::ui;

//Simple initaliser
UIElement::UIElement(vec2 pos, vec2 org, vec2 size, debug::Logger* log, const std::string& name) {
	//Debug information
	logger = log;
	this->name = name;
	logger->registerClass(this, "[UIE]" + name);
	logger->info(this, "Initalising element...");
	//Register size of element
	rect = new UIRect(pos, org, size);
	//Initalise
	element = new UICluster;
	//Update references, states and intialise rest of the variables
	currentState = UIState::INACTIVE;
	counter = 0;
	internalClock = sf::Clock();
	states = std::map<UIState, std::string>();
	states.insert({ UIState::ACTIVE, ""});
	states.insert({ UIState::HOVER,  ""});
	states.insert({ UIState::LOCK,   ""});
	states.insert({ UIState::CLICK,  ""});
	states.insert({ UIState::TRUE,   ""});
	states.insert({ UIState::FALSE,  ""});
	//Inform end of initalisation
	logger->info(this, "Finished initalising element.");
}

//Simple get for name
std::string UIElement::getName() {
	return this->name;
}

//Simple set for name
void UIElement::setName(const std::string& newname) {
	this->name = newname;
}

//Move entire UIElement by Vector vec
void UIElement::move(vec2 vec) {
	rect->pos += vec;

	//Loop through each entity and move valid (not deleted) entities by vec
	for (auto& spr : *element->spr->data) {
		if (spr != nullptr) {
			spr->move(vec.x, vec.y);
		}
	}

	for (auto& txt : *element->txt->data) {
		if (txt != nullptr) {
			txt->move(vec.x, vec.y);
		}
	}

	for (auto& shp : *element->shp->data) {
		if (shp != nullptr) {
			shp->move(vec.x, vec.y);
		}
	}
}

//Set position to be <pos> by moving from current pos by calculated difference
void UIElement::setPosition(vec2 pos) {
	vec2 move = rect->pos - pos;
	this->move(vec2(-move.x, -move.y));
}

//Empty reference array
void UIElement::cleanup() {
	states.empty();
}

//Default destructor
UIElement::~UIElement() {
	cleanup();
	element->~UICluster();
	delete& states;
	ref_m_asset = nullptr;
}

//Simple method to dynamically allocate state and texture binding
void UIElement::setStateTex(UIState s, std::string ref) {
	switch (s) {
	case UIState::ACTIVE:
		states.at(s) = ref;
		break;
	case UIState::HOVER:
		states.at(s) = ref;
		break;
	case UIState::LOCK:
		states.at(s) = ref;
		break;
	case UIState::CLICK:
		states.at(s) = ref;
		break;
	case UIState::TRUE:
		states.at(s) = ref;
	case UIState::FALSE:
		states.at(s) = ref;
	default:
		break;
	}
}

//Empty initaliser - UIElement child classes will override this method
void UIElement::init() {

}

//Empty update method - UIElement child classes will override this method
void UIElement::update(sf::RenderWindow *win) {

}

//Simple method to get reference to current state of this element
UIState* UIElement::getState() {
	return &currentState;
}

//Simple method to override current state of this element
void UIElement::setState(UIState s) {
	currentState = s;
}

//Method for external classes to reference the sprite associated with this element
UICluster* UIElement::getCluster() {
	return element;
}

//Set UIE state to locked (if unlocked)
void UIElement::lock() {
	if (!locked) {
		locked = true;
		setState(UIState::LOCK);
	}
}

//Set UIE state to unlocked (if locked and NOT paused)
void UIElement::unlock() {
	if (locked && !paused) {
		locked = false;
	}
}

//"Pause" (lock, wait and unlock) 
void UIElement::pause(float time) {
	if (!locked) {
		//Lock this sprite
		lock();
		//Set pause flag
		paused = true;
		//Increase counter by accumulated time since last pause
		counter += internalClock.getElapsedTime().asSeconds();
		//Restart clock
		internalClock.restart();
		//Launch unlock control thread
		std::thread pauseThread(&UIElement::countdownTimer, this, time);
	}
}

//Unlock Control Method
//**INTENDED TO BE USED WITHIN A THREAD!!**
//Will unlock infinte loop as soon as counter+time time is reached
void UIElement::countdownTimer(float time) {
	do{
		//Get time, if more than/equal to counter+time, unlock & break loop
		if ((internalClock.getElapsedTime().asSeconds() >= (counter + time))) {
			unlock();
			paused = false;
			break;
		}
	} while (internalClock.getElapsedTime().asSeconds() <= (counter + time));

	//Just in case time is weirded out, check one last time if unlock condition needs to be set
	if (paused) {
		if ((internalClock.getElapsedTime().asSeconds() >= (counter + time))) {
			unlock();
			paused = false;
		}
	}
}

//Setter for activity state
void UIElement::setActive(bool state) {
	active = state;
}

//Getter for activity state
bool UIElement::isActive() {
	return active;
}

//Get sprite by raw ID reference (like raw vector)
sf::Sprite* UIElement::getSpriteByID(int id) {
	return (sf::Sprite*)getRawElementByID(id, UIObject::SPRITE);
}

//Get shape by raw ID reference (like raw vector)
sf::Shape* UIElement::getShapeByID(int id) {
	return (sf::Shape*)getRawElementByID(id, UIObject::SHAPE);
}

//Get text by raw ID reference (like raw vector)
sf::Text* UIElement::getTextByID(int id) {
	return (sf::Text*)getRawElementByID(id, UIObject::TEXT);
}

//Get sprite by name reference
sf::Sprite* UIElement::getSpriteByName(const std::string& name) {
	return (sf::Sprite*)getRawElementByName(name, UIObject::SPRITE);
}

//Get shape by name reference
sf::Shape* UIElement::getShapeByName(const std::string& name) {
	return (sf::Shape*)getRawElementByName(name, UIObject::SHAPE);
}

//Get text by name reference
sf::Text* UIElement::getTextByName(const std::string& name) {
	return (sf::Text*)getRawElementByName(name, UIObject::TEXT);
}

//Get element of <type> by raw id <id> (like raw vector)
void* UIElement::getRawElementByID(int id, UIObject type) {
	try {
		switch (type) {
		case UIObject::SPR:
			return element->spr->data->at(id);
		case UIObject::SHP:
			return element->shp->data->at(id);
		case UIObject::TXT:
			return element->txt->data->at(id);
		default:
			return nullptr;
		}
	}
	catch (std::exception e) {
		return nullptr;
	}
}

//Get element of <type> by raw id gained from <name>
void* UIElement::getRawElementByName(const std::string& name, UIObject type) {
	try {
		switch (type) {
		case UIObject::SPR:
			return element->spr->data->at(element->spr->id_ref->at(name));
		case UIObject::SHP:
			return element->shp->data->at(element->shp->id_ref->at(name));
		case UIObject::TXT:
			return element->txt->data->at(element->txt->id_ref->at(name));
		default:
			return nullptr;
		}
	}
	catch (std::exception e) {
		return nullptr;
	}
}

bool UIElement::addSprite(const std::string& name, sf::Sprite* what, bool center) {
	return addComponent(name, (void*)what, UIObject::SPRITE, center);
}

bool UIElement::addShape(const std::string& name, sf::Shape* what, bool center) {
	return addComponent(name, (void*)what, UIObject::SHAPE, center);
}

bool UIElement::addText(const std::string& name, sf::Text* what, bool center) {
	return addComponent(name, (void*)what, UIObject::TEXT, center);
}

//Sprite-typed center method by name
void UIElement::centerSprite(const std::string& name) {
	centerComponent(name, UIObject::SPRITE);
}

//Text-typed center method by name
void UIElement::centerText(const std::string& name) {
	centerComponent(name, UIObject::TEXT);
}

//Shape-typed center method by name
void UIElement::centerShape(const std::string& name) {
	centerComponent(name, UIObject::SHAPE);
}

//Sprite-typed center method by pointer
void UIElement::centerSprite(sf::Sprite* obj) {
	centerComponent(obj, UIObject::SPRITE);
}

//Text-typed center method by pointer
void UIElement::centerText(sf::Text* obj) {
	centerComponent(obj, UIObject::TEXT);
}

//Shape-typed center method by pointer
void UIElement::centerShape(sf::Shape* obj) {
	centerComponent(obj, UIObject::SHAPE);
}


//Get for UIRect position
vec2* UIElement::getPosition() {
	return &rect->pos;
}

//Get for UIRect origin
vec2* UIElement::getOrigin() {
	return &rect->origin;
}

//Get for UIRect size
vec2* UIElement::getSize() {
	return &rect->size;
}

void UIElement::reCenter() {
	for (auto& spr : *element->spr->data) {
		centerSprite(spr);
	}
	for (auto& txt : *element->txt->data) {
		centerText(txt);
	}
	for (auto& shp : *element->shp->data) {
		centerShape(shp);
	}
}

//Method to center components
void UIElement::centerComponent(const std::string& name, UIObject type) {
	switch (type) {
	case UIObject::SPR:
		//Get local reference to pointer
		sf::Sprite* spr_ptr;
		spr_ptr = getSpriteByName(name);
		//If pointer found, set origin based on HALF width and HALF weight
		if (spr_ptr != nullptr) {
			spr_ptr->setOrigin(spr_ptr->getGlobalBounds().width / 2, spr_ptr->getGlobalBounds().height / 2);
		}
		break;
	case UIObject::TXT:
		sf::Text* txt_ptr;
		txt_ptr = getTextByName(name);
		if (txt_ptr != nullptr) {
			txt_ptr->setOrigin(txt_ptr->getGlobalBounds().width / 2, txt_ptr->getGlobalBounds().height / 2);
		}
		break;
	case UIObject::SHP:
		sf::Shape* shp_ptr;
		shp_ptr = getShapeByName(name);
		if (shp_ptr != nullptr) {
			shp_ptr->setOrigin(shp_ptr->getGlobalBounds().width / 2, shp_ptr->getGlobalBounds().height / 2);
		}
		break;
	}
}

//Method to center
void UIElement::centerComponent(void* obj, UIObject type) {
	try {
		switch (type) {
		case UIObject::SPR:
			//Cast passed pointer
			sf::Sprite* spr_ptr;
			spr_ptr = (sf::Sprite*)obj;
			//If pointer found, set origin based on HALF width and HALF weight
			if (spr_ptr != nullptr) {
				spr_ptr->setOrigin(spr_ptr->getGlobalBounds().width / 2, spr_ptr->getGlobalBounds().height / 2);
			}
			break;
		case UIObject::TXT:
			sf::Text* txt_ptr;
			txt_ptr = (sf::Text*)obj;
			if (txt_ptr != nullptr) {
				txt_ptr->setOrigin(txt_ptr->getGlobalBounds().width / 2, txt_ptr->getGlobalBounds().height / 2);
			}
			break;
		case UIObject::SHP:
			sf::Shape* shp_ptr;
			shp_ptr = (sf::Shape*)obj;
			if (shp_ptr != nullptr) {
				shp_ptr->setOrigin(shp_ptr->getGlobalBounds().width / 2, shp_ptr->getGlobalBounds().height / 2);
			}
			break;
		}
	}
	catch (std::exception e) {
		logger->warn(this, "ERROR CENTERING COMPONENT ");
	}
}

//Add element of <type> found at <obj> under <name>. Registers both human-friendly version as well as in the vector
//Return TRUE on error-less completion, return FALSE if any errors occured.
bool UIElement::addComponent(const std::string& name, void* obj, UIObject type, bool center) {
	try {
		switch (type) {
		case UIObject::SPR:
			//See if element found under same name
			//If element found, overwrite contents
			if (element->spr->id_ref->find(name) != element->spr->id_ref->end()) {
				element->spr->data->at(element->spr->id_ref->find(name)->second) = (sf::Sprite*)obj;
			}
			//If element not found, add as new
			else {
				element->spr->data->push_back((sf::Sprite*)obj);
				element->spr->id_ref->insert({ name, element->spr->data->size() - 1 });
			}

			if (center) {
				centerSprite(name);
			}

			return true;
		case UIObject::SHP:
			//See if element found under same name
			//If element found, overwrite contents
			if (element->shp->id_ref->find(name) != element->shp->id_ref->end()) {
				element->shp->data->at(element->shp->id_ref->find(name)->second) = (sf::Shape*)obj;
			}
			//If element not found, add as new
			else {
				element->shp->data->push_back((sf::Shape*)obj);
				element->shp->id_ref->insert({ name, element->shp->data->size() - 1 });
			}

			if (center) {
				centerShape(name);
			}

			return true;
		case UIObject::TXT:
			if (element->txt->id_ref->find(name) != element->txt->id_ref->end()) {
				element->txt->data->at(element->txt->id_ref->find(name)->second) = (sf::Text*)obj;
			}
			else {
				element->txt->data->push_back((sf::Text*)obj);
				element->txt->id_ref->insert({ name, element->txt->data->size() - 1 });
			}

			if (center) {
				centerText(name);
			}

			return true;
		default:
			return false;
		}
	}
	catch (std::exception e) {
		logger->info(this, "Could not add UICluser Composite <" + name + ">! Detail: " + e.what());
		return false;
	}
}

//Delete element <name> of <type>
bool UIElement::delComponent(const std::string& name, UIObject type) {
	try {
		switch (type) {
		case UIObject::SPR:
			delete this->getSpriteByName(name);
			return true;
		case UIObject::TXT:
			delete this->getTextByName(name);
			return true;
		case UIObject::SHP:
			delete this->getShapeByName(name);
			return true;
		default:
			return false;
		}
	}
	catch (std::exception e) {
		logger->info(this, "Could not delete UICluser Composite <" + name + ">! Detail: " + e.what());
		return false;
	}
}

//Simple draw method
//Draws Shapes from 0-x, then Sprites from 0-x, and finally Text from 0-x
void UIElement::draw(sf::RenderWindow* w){
	int i = 0;
	for (i = 0; i < element->shp->data->size(); i++) {
		if (element->shp->data->at(i) != nullptr) {
			w->draw(*element->shp->data->at(i));
		}
	}

	for (i = 0; i < element->spr->data->size(); i++) {
		if (element->spr->data->at(i) != nullptr) {
			w->draw(*element->spr->data->at(i));
		}
	}

	for (i = 0; i < element->txt->data->size(); i++) {
		if (element->txt->data->at(i) != nullptr) {
			w->draw(*element->txt->data->at(i));
		}
	}
}