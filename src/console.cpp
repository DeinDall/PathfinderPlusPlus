#include "console.h"

#define CONSOLE_LINE_HEIGHT 24

Console::Console() {
	mFont.loadFromFile("resources/font.ttf");
}

Console::~Console() {
	for (ConsoleLine* line : mLines)
		delete line;
}

void Console::addText(std::wstring text) {
	ConsoleLine* line = new ConsoleLine;

	line->text.setFont(mFont);
	line->text.setCharacterSize(16);
	line->text.setString(sf::String(text));
	line->text.setColor(sf::Color::Red);

	line->timeout = 180;
	line->y = -CONSOLE_LINE_HEIGHT;

	mLines.push_front(line);
}

void Console::update() {
	int i=0;
	for (ConsoleLine* line : mLines) {
		if (line->y < i*CONSOLE_LINE_HEIGHT)
			line->y -= ((line->y - i*CONSOLE_LINE_HEIGHT)/8)-1;

		line->timeout--;

		if (line->timeout==0) {
			mLines.remove(line);
			delete line;
		} else if (line->timeout<60)
			line->text.setColor(sf::Color(255, 0, 0, (line->timeout/60.f)*255));

		line->text.setPosition(sf::Vector2f(8, 8+line->y));

		++i;
	}
}

void Console::draw(sf::RenderWindow& window) {
	for (ConsoleLine* line : mLines)
		window.draw(line->text);
}
