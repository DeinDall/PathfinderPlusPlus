#ifndef PATHINFO_H
#define PATHINFO_H

#include <SFML/Graphics.hpp>
#include <sstream>

class PathInfo {
public:
	PathInfo(sf::Font& font, sf::Vector2i pos);

	void setInformations(int pathSize, float time, int cCount);

	void draw(sf::RenderWindow& window);

private:
	sf::Text mText;
};

#endif // PATHINFO_H
