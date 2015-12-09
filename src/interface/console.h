#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>

#include <iostream>

#define CONSOLE_LINE_HEIGHT 24

class Console {
	struct ConsoleLine {
		sf::Text text;
		int timeout;
		int y;
	};

public:
	Console(sf::Font& font);
	~Console();

	void addText(const std::string& text);

	void update();

	void draw(sf::RenderWindow& window);

private:
	sf::Font& mFont;
	std::list<ConsoleLine*> mLines;
};

#endif // CONSOLE_H_INCLUDED
