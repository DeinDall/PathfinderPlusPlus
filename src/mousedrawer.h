#ifndef MOUSEDRAWER_H
#define MOUSEDRAWER_H

#include "map.h"

class MouseDrawer {
public:
	enum DrawMethod {
		DRAW_FREE,
		DRAW_LINES,
		DRAW_RECT,
		DrawMethodCount
	};

public:
	MouseDrawer(sf::IntRect drawArea);

	void onMousePress(sf::Vector2i mpos, bool erase);

	void onMouseRelease(sf::Vector2i mpos, bool erase);

	void setColor(const sf::Color& color);

	void update(sf::Vector2i mpos, Map& map);

	void draw(sf::RenderWindow& window);

private:
	bool mDragging, mEraseMode;
	DrawMethod mDrawMethod;
	sf::Vector2i mStartPos, mCurrentPos;
	sf::IntRect mDrawArea;
	sf::Color mColor;
};

#endif // MOUSEDRAWER_H
