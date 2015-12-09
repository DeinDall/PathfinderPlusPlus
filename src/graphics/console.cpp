#include "console.h"

const int Console::msLineHeight = 24;

Console::Console(std::shared_ptr<const sf::Font> font) : mFont(font) {
	addText(L"Programme lancé");
}

Console::~Console() {
	for (ConsoleLine* line : mLines)
		delete line;
}

void Console::addText(const sf::String& text) {
	ConsoleLine* line = new ConsoleLine;

	line->text.setFont(*mFont);
	line->text.setCharacterSize(16);
	line->text.setString(text);
	line->text.setColor(sf::Color::Red);

	line->timeout = 160;
	line->y = -msLineHeight;

	mLines.push_front(line);
}

void Console::update() {
	int i=0;

	for (auto it = mLines.begin(); it != mLines.end(); ++it) {
		ConsoleLine* line = *it;

		int goal = i*msLineHeight;

		if (line->y < goal)
			line->y += ((goal - line->y)/8)+1;

		line->timeout--;

		if (line->timeout==0) {
			mLines.erase(it--);
			delete line;
		} else {
			if (line->timeout<40)
				line->text.setColor(sf::Color(255, 0, 0, (line->timeout/40.f)*255));
			line->text.setPosition(sf::Vector2f(8, 8+line->y));
		}

		++i;
	}
}

void Console::draw(sf::RenderWindow& window) {
	for (ConsoleLine* line : mLines)
		window.draw(line->text);
}
