#include <SFML/Graphics.hpp>
#include <sstream>

#include "pathfinder.h"
#include "console.h"

int main(int argc, char** argv) {
	Map map(24, 18);

	if (argc>1)
		map.load(std::string(argv[1]));

	sf::RenderWindow window(sf::VideoMode(1024, 576), "ISN : Recherche de chemin", sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Event ev;

	Console console;

	Pathfinder pathfinder;

	// Les zones de la fenêtre dans lesquelles nous avons les différents éléments
	sf::IntRect mapViewRect(0, 0, 768, 576);
	sf::IntRect guiViewRect(768, 0, 256, 576);

	while (window.isOpen()) {
		while (window.pollEvent(ev)) {
			if (ev.type==sf::Event::Closed)
				window.close();
			else if (ev.type==sf::Event::MouseButtonPressed) {
				if (mapViewRect.contains(ev.mouseButton.x, ev.mouseButton.y)) {
					if (ev.mouseButton.button==sf::Mouse::Right) {
						sf::Clock clock;

						pathfinder.computePath(map, 0, 0, ev.mouseButton.x/32, ev.mouseButton.y/32);

						sf::Time time = clock.getElapsedTime();

						pathfinder.computePathGraphics();

						std::wstringstream wss;
						wss << L"Chemin calculé en " << time.asMicroseconds() << L"µs";

						console.addText(wss.str());
					} else if (ev.mouseButton.button==sf::Mouse::Left)
						map.set(ev.mouseButton.x/32, ev.mouseButton.y/32, (!map.get(ev.mouseButton.x/32, ev.mouseButton.y/32))*8);
				} else if (guiViewRect.contains(ev.mouseButton.x, ev.mouseButton.y)){
					// Trucs de l'Interface ici
				}
			} else if (ev.type==sf::Event::KeyPressed) {
				if (ev.key.code==sf::Keyboard::S) {
					map.save();
					console.addText(L"Map sauvegardee sous \"pathfindermap.srpfmp\"");
				}
			}
		}

		console.update();

		window.clear();

		map.draw(window);

		for (int i=0; i<map.height(); ++i) {
			map.drawWallLayer(window, i);
			// if (player.y == i) player.draw(window);
		}

		if (!pathfinder.path().empty())
			pathfinder.draw(window);

        console.draw(window);

		window.display();
	}

	return 0;
}
