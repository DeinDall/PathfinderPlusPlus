#include "mousedrawer.h"

MouseDrawer::MouseDrawer(sf::IntRect drawArea) : mDrawArea(drawArea) {
	mDragging = false;
	mEraseMode = false;
	mShouldDraw = false;
	mDrawMethod = DRAW_FREE;
}

void MouseDrawer::setColor(const sf::Color& color) {
	mColor = color;
}

void MouseDrawer::setMethod(DrawMethod method) {
	mDrawMethod = method;
}

void MouseDrawer::onMousePress(sf::Vector2i mpos, bool erase) {
	mStartPos = mpos/32;
	mDragging = true;
	mEraseMode = erase;
}

void MouseDrawer::onMouseRelease(sf::Vector2i mpos, bool erase) {
	if (mDragging && mEraseMode==erase) {
		mDragging = false;
		mShouldDraw = true;
	}
}

void MouseDrawer::update(sf::Vector2i mpos, Map& map) {
	if (!mDrawArea.contains(mpos)) {
		mDragging = false;
	} else {
		mCurrentPos = mpos/32;

		if (mDragging) {
			switch (mDrawMethod) {
			case DRAW_FREE:
				for (sf::Vector2i pos : getLineBetween(mStartPos, mCurrentPos))
					map.set(pos.x, pos.y, !mEraseMode);
				mStartPos = mCurrentPos;
				break;

			case DRAW_LINES:
			case DRAW_RECT:
				break;

			default:
				break;
			}
		} else if (mShouldDraw) {
			switch (mDrawMethod) {
			case DRAW_FREE:
				break;
			case DRAW_LINES:
				for (sf::Vector2i pos : getLineBetween(mStartPos, mCurrentPos))
					map.set(pos.x, pos.y, !mEraseMode);
				break;
			case DRAW_RECT:
				break;

			default:
				break;
			}
			mShouldDraw=false;
		} else {
			mStartPos = mCurrentPos;
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
		sf::RectangleShape rect(sf::Vector2f(32, 32));
		rect.setFillColor(mColor); // sf::Color(64, 128, 255, 128)

		for (sf::Vector2i pos : getLineBetween(mStartPos, mCurrentPos)) {
			rect.setPosition(pos.x*32, pos.y*32);
			window.draw(rect);
		}
		break;
	}
	case DRAW_RECT: {
		break;
	}
	}
}

std::vector<sf::Vector2i> MouseDrawer::getLineBetween(sf::Vector2i start, sf::Vector2i end) const {
	std::vector<sf::Vector2i> poslist;
	sf::Vector2i diff = (end-start);
	unsigned int size = std::max(std::abs(diff.x), std::abs(diff.y));

	poslist.resize(size+1);
	poslist[size] = end;

	for (unsigned int i=0; i<size; ++i) {
		float a=((float)(i+0.5)/size);
		poslist[i] = start+sf::Vector2i(diff.x*a, diff.y*a);
	}

	return std::move(poslist);
}
