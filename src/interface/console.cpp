#include "console.h"

#define CONSOLE_LINE_HEIGHT 24

Console::Console(sf::Font& font) : mFont(font) {
	addText(L"Programme lancé");
}

Console::~Console() {
	for (ConsoleLine* line : mLines)
		delete line;
}

void Console::addText(const std::wstring& text) {
	ConsoleLine* line = new ConsoleLine;

	line->text.setFont(mFont);
	line->text.setCharacterSize(16);
	line->text.setString(sf::String(text));
	line->text.setColor(sf::Color::Red);

	line->timeout = 160;
	line->y = -CONSOLE_LINE_HEIGHT;

	mLines.push_front(line);
}

void Console::update() {
	int i=0;
	std::vector<std::list<ConsoleLine*>::iterator> toDelete;

	for (auto it = mLines.begin(); it != mLines.end(); ++it) {
		ConsoleLine* line = *it;

		if (line->y < i*CONSOLE_LINE_HEIGHT)
			line->y -= ((line->y - i*CONSOLE_LINE_HEIGHT)/8)-1;

		line->timeout--;

		if (line->timeout==0) {
			toDelete.push_back(it);
			delete line;
		} else {
			if (line->timeout<40)
				line->text.setColor(sf::Color(255, 0, 0, (line->timeout/40.f)*255));
			line->text.setPosition(sf::Vector2f(8, 8+line->y));
		}

		++i;
	}

	for (auto it : toDelete)
		mLines.erase(it);
}

void Console::draw(sf::RenderWindow& window) {
	for (ConsoleLine* line : mLines)
		window.draw(line->text);
}
