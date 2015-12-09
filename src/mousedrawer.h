#ifndef MOUSEDRAWER_H
#define MOUSEDRAWER_H

#include <SFML/Graphics.hpp>
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

	void setColor(const sf::Color& color);
	void setMethod(DrawMethod method);

	void onMousePress(sf::Vector2i mpos, bool erase);
	void onMouseRelease(sf::Vector2i mpos, bool erase);

	void update(sf::Vector2i mpos, Map& map);
	void draw(sf::RenderWindow& window);

protected:
	std::vector<sf::Vector2i> getLineBetween(sf::Vector2i start, sf::Vector2i end) const;

private:
	bool mDragging, mEraseMode, mShouldDraw;
	DrawMethod mDrawMethod;
	sf::Vector2i mStartPos, mCurrentPos;
	sf::IntRect mDrawArea;
	sf::Color mColor;
};

#endif // MOUSEDRAWER_H
