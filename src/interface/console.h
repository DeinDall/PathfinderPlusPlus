#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>

#include <iostream>

class Console {
	struct ConsoleLine {
		sf::Text text;
		int timeout;
		int y;
	};

public:
	Console(sf::Font& font);
	~Console();

	void addText(const std::wstring& text);

	void update();
	void draw(sf::RenderWindow& window);

private:
	sf::Font& mFont;
	std::list<ConsoleLine*> mLines;
};

#endif // CONSOLE_H_INCLUDED
