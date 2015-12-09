#include "mousedrawer.h"

MouseDrawer::MouseDrawer(sf::IntRect drawArea) : mDrawArea(drawArea) {
	mDragging = false;
	mEraseMode = false;
	mDrawMethod = DRAW_FREE;
}

void MouseDrawer::onMousePress(sf::Vector2i mpos, bool erase) {
	mStartPos = mpos/32;
	mDragging = true;
	mEraseMode = erase;
}

void MouseDrawer::onMouseRelease(sf::Vector2i mpos, bool erase) {
	if (mDragging && mEraseMode==erase) {
		mDragging = false;
	}
}

void MouseDrawer::setColor(const sf::Color& color) {
	mColor = color;
}

void MouseDrawer::update(sf::Vector2i mpos, Map& map) {
	if (!mDrawArea.contains(mpos)) {
		mDragging = false;
	} else {
		mCurrentPos = mpos/32;

		if (mDragging) {
			switch (mDrawMethod) {
			case DRAW_FREE:
				mStartPos = mCurrentPos;
				map.set(mCurrentPos.x, mCurrentPos.y, !mEraseMode);
				break;

			case DRAW_LINES:
			case DRAW_RECT:
				break;

			default:
				break;
			}
		}
	}
}

void MouseDrawer::draw(sf::RenderWindow& window) {
	switch (mDrawMethod) {
	case DRAW_FREE: {
		sf::RectangleShape rect(sf::Vector2f(32, 32));

		rect.setFillColor(mColor); // sf::Color(64, 128, 255, 128)
		rect.setPosition(mCurrentPos.x*32, mCurrentPos.y*32);

		window.draw(rect);
		break;
	}

	case DRAW_LINES: {
		break;
	}
	case DRAW_RECT: {
		break;
	}
	}
}
