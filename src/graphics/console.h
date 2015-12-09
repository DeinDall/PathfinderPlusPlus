#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <SFML/Graphics.hpp>

#include <list>
#include <memory>

class Console {
	struct ConsoleLine {
		sf::Text text;
		int timeout;
		int y;
	};

public:
	Console(std::shared_ptr<const sf::Font> font);
	~Console();

	void addText(const sf::String& text);

	void update();

	void draw(sf::RenderWindow& window);

private:
	std::shared_ptr<const sf::Font> mFont;
	std::list<ConsoleLine*> mLines;

	static const int msLineHeight;
};

#endif // CONSOLE_H_INCLUDED
