#include <SFML/Graphics.hpp>
#include <sstream>

#include "pathfinder.h"

int main(int argc, char** argv) {
	Map map(32, 18);

	if (argc>1)
		map.load(std::string(argv[1]));

	sf::RenderWindow window(sf::VideoMode(1024, 576), "ISN : Recherche de chemin", sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Event ev;

	sf::Font font;
	font.loadFromFile("font.ttf");

	sf::Text text("", font, 16);
	text.setPosition(8, 8);
	text.setColor(sf::Color::Red);

	int textTimeout = 0;

	Pathfinder pathfinder;

	for (unsigned int ix=0; ix<map.width(); ++ix)
		for (unsigned int iy=0; iy<map.height(); ++iy)
			map.set(ix, iy, 0);

	static const sf::Color colors[4] = { sf::Color::White, sf::Color::Black, sf::Color::Blue, sf::Color::Green };

	while (window.isOpen()) {
		while (window.pollEvent(ev)) {
			if (ev.type==sf::Event::Closed)
				window.close();
			else if (ev.type==sf::Event::MouseButtonPressed) {
				if (ev.mouseButton.button==sf::Mouse::Right) {
					sf::Clock clock;

					pathfinder.computePath(map, 0, 0, ev.mouseButton.x/32, ev.mouseButton.y/32);

					sf::Time time = clock.getElapsedTime();

					std::wstringstream wss;
					wss << L"Chemin calculé en " << time.asMicroseconds() << L"µs";

					text.setString(sf::String(wss.str()));
					textTimeout = 180;
				} else if (ev.mouseButton.button==sf::Mouse::Left)
					map.set(ev.mouseButton.x/32, ev.mouseButton.y/32, !map.get(ev.mouseButton.x/32, ev.mouseButton.y/32));
			} else if (ev.type==sf::Event::KeyPressed) {
				if (ev.key.code==sf::Keyboard::S) {
					map.save();
					text.setString(L"Map sauvegardée sous \"pathfindermap.srpfmp\"");
					textTimeout = 180;
				}
			}
		}

		if (textTimeout>0) {
			textTimeout--;
			if (textTimeout==0)
				text.setString("");
		}


		window.clear();

		sf::RectangleShape shape;
		shape.setSize(sf::Vector2f(32, 32));

		for (unsigned int ix=0; ix<map.width(); ++ix)
			for (unsigned int iy=0; iy<map.height(); ++iy) {
				shape.setPosition(sf::Vector2f(ix*32, iy*32));
				shape.setFillColor(colors[map.get(ix, iy)]);
				window.draw(shape);
			}

		if (!pathfinder.path().empty()) {
			sf::Vector2i offset(0, 0);
			sf::RectangleShape shape;

			shape.setSize(sf::Vector2f(32, 32));
			shape.setFillColor(sf::Color(64, 255, 0, 128));

			for (Direction dir : pathfinder.path()) {
				offset.x += dir.xOffset;
				offset.y += dir.yOffset;

				shape.setPosition(offset.x*32, offset.y*32);
				window.draw(shape);
			}
		}

		window.draw(text);

		window.display();
	}

	return 0;
}
