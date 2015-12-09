#include "buttons.h"

Buttons::Buttons(sf::Vector2u offset, int width) {
	mButtonList.resize(ButtonCount);

	sf::IntRect currentRect(offset.x+16, offset.y+16, width-32, 32);

	for (int i=0; i<ButtonCount; ++i) {
		mButtonList[i] = new Button(currentRect, i);
		currentRect.top += 40;
	}

	mVerticalOffset = currentRect.top;
}

Buttons::~Buttons() {
	for (Button* button : mButtonList)
		delete button;
}

Button& Buttons::getButtonFromId(ButtonId id) {
	return *(mButtonList[id]);
}

void Buttons::onMousePress(sf::Vector2i pos) {
	for (Button* button : mButtonList)
		button->onMousePressed(pos);
}

void Buttons::onMouseRelease(sf::Vector2i pos) {
	for (Button* button : mButtonList)
		button->onMouseReleased(pos);
}

void Buttons::update(sf::Vector2i pos) {
	for (Button* button : mButtonList) {
		button->update(pos);
		if (button->getClicked())
			mClickedIds.push((ButtonId) button->getId());
	}
}

void Buttons::draw(sf::RenderWindow& window) {
	for (Button* button : mButtonList)
		button->draw(window);
}

bool Buttons::popClick(ButtonId& id) {
	if (!mClickedIds.empty()) {
		id = mClickedIds.front();
		mClickedIds.pop();
		return true;
	}
	return false;
}

int Buttons::verticalOffset() {
	return mVerticalOffset;
}
