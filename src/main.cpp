#include <SFML/Graphics.hpp>
#include <sstream>

#include "pathfinder.h"
#include "mousedrawer.h"

#include "interface/stk_fileopenwrapper.h"

#include "interface/console.h"
#include "interface/buttons.h"
#include "interface/pathinfo.h"

#include <iostream>

int main(int, char**) {
	// La carte, qui contient tous les bloc sous forme booléene.
	Map map(24, 18);

	sf::RenderWindow window(sf::VideoMode(1024, 576), "Pathfinder", sf::Style::Close);
	window.setFramerateLimit(60);

	// Les zones de la fenêtre dans lesquelles nous avons les différents éléments
	sf::IntRect mapViewRect(0, 0, 768, 576);
	sf::IntRect guiViewRect(768, 0, 256, 576);

	// Les containeurs d'évènements. ev pour la fenêtre, et buttonEv pour les boutons.
	sf::Event ev;
	Buttons::ButtonId buttonEv;

	// settingStart: true si l'utilisateur met le début, settingSearch: true si l'utilisateur met la fin.
	bool settingStart=false, settingSearch=false;

	// Les positions de début et de fin de recherche.
	sf::Vector2i startPos(6, 4), endPos(map.width()-7, map.height()-5);

	// L'instance de la classe Buttons, s'occupe des boutons.
	Buttons buttons(sf::Vector2u(guiViewRect.left, guiViewRect.top), guiViewRect.width);

	// La "Console", qui gère les textes s'affichant en haut à gauche.
	Console console(Button::getFont());

	// shouldUpdatePath: true si le chemin doit être recalculé
	bool shouldUpdatePath=true;

	// showAdvancedPath: true si l'on affiche le poids de chaque case ayant été vérifiée (Pathfinder)
	bool showAdvancedPath=false;

	// L'instance de la classe Pathfinder, calcule le chemin.
	Pathfinder pathfinder;

	// L'instance de la classe MouseDrawer, gère le dessin à la souris.
	MouseDrawer drawer(mapViewRect);

	PathInfo pathInfo(Button::getFont(), sf::Vector2i(guiViewRect.left+16, buttons.verticalOffset()));

	// Ici, on met le texte sur les boutons.
	buttons.getButtonFromId(Buttons::BTN_SHOWCASES).setContents(L"Afficher le poids des cases");
	buttons.getButtonFromId(Buttons::BTN_CLEAR).setContents(L"Remettre à zero");
	buttons.getButtonFromId(Buttons::BTN_RECOMPUTE).setContents(L"Rafraichir le chemin");
	buttons.getButtonFromId(Buttons::BTN_SETSTART).setContents(L"Placer le début");
	buttons.getButtonFromId(Buttons::BTN_PATHSEARCH).setContents(L"Placer la fin");
	buttons.getButtonFromId(Buttons::BTN_LOAD).setContents(L"Ouvrir");
	buttons.getButtonFromId(Buttons::BTN_SAVE).setContents(L"Enregistrer Sous");

	while (window.isOpen()) {
		// Ici, on commence à parcourir les évènements envoyés par la fenêtre
		while (window.pollEvent(ev)) {
			if (ev.type==sf::Event::Closed)
				window.close();
			else if (ev.type==sf::Event::MouseButtonPressed) {
				sf::Vector2i mpos(ev.mouseButton.x, ev.mouseButton.y);
				if (mapViewRect.contains(mpos)) {
					if (settingStart || settingSearch)
						break;

					if (ev.mouseButton.button==sf::Mouse::Right) {
						drawer.onMousePress(mpos, true);
					} else if (ev.mouseButton.button==sf::Mouse::Left) {
						drawer.onMousePress(mpos, false);
					}
				} else if (guiViewRect.contains(ev.mouseButton.x, ev.mouseButton.y)){
					if (ev.mouseButton.button==sf::Mouse::Left)
						buttons.onMousePress(mpos);
				}
			} else if (ev.type==sf::Event::MouseButtonReleased) {
				sf::Vector2i mpos(ev.mouseButton.x, ev.mouseButton.y);

				if (mapViewRect.contains(mpos)) {
					if (settingSearch) {
						endPos = mpos/32;
						shouldUpdatePath=true;
						settingSearch = false;
					} else if (settingStart) {
						startPos = mpos/32;
						shouldUpdatePath=true;
						settingStart = false;
					} else if (ev.mouseButton.button==sf::Mouse::Right) {
						drawer.onMouseRelease(mpos, true);
					} else if (ev.mouseButton.button==sf::Mouse::Left) {
						drawer.onMouseRelease(mpos, false);
					}
				} else if (guiViewRect.contains(ev.mouseButton.x, ev.mouseButton.y)) {
					if (ev.mouseButton.button==sf::Mouse::Left)
						buttons.onMouseRelease(mpos);
				}
			}
		}

		buttons.update(sf::Mouse::getPosition(window));
		drawer.update(sf::Mouse::getPosition(window), map);

		while (buttons.popClick(buttonEv)) {
			std::wstring fname;
			switch (buttonEv) {
			case Buttons::BTN_SHOWCASES:
				showAdvancedPath = !showAdvancedPath;

				if (showAdvancedPath)
					buttons.getButtonFromId(Buttons::BTN_SHOWCASES).setContents(L"Cacher le poids des cases");
				else
					buttons.getButtonFromId(Buttons::BTN_SHOWCASES).setContents(L"Afficher le poids des cases");
				break;
			case Buttons::BTN_CLEAR:
				if (stk::yesNoWarningMessage(window, L"Voulez-vous vraiment supprimer toute la carte ?"))
					map.clear();
				break;

			case Buttons::BTN_RECOMPUTE:
				shouldUpdatePath=true;
				break;

			case Buttons::BTN_SAVE:
				fname = stk::getSaveFileName(window);

				if (fname != L"error") {
					map.save(fname);
					console.addText(L"Fichier enregistré");
				} else
					console.addText(L"Erreur lors de la sauvegarde");
				break;
			case Buttons::BTN_LOAD:
				fname = stk::getOpenFileName(window);

				if (fname != L"error") {
					map.load(fname);
					console.addText(L"Fichier chargé");
				} else
					console.addText(L"Erreur lors du chargement");
				break;

			case Buttons::BTN_SETSTART:
				settingStart = true;
				settingSearch = false;
				break;

			case Buttons::BTN_PATHSEARCH:
				settingStart = false;
				settingSearch = true;
				break;

			default:
				break;
			}
		}

		// Ici, on met les cases du début et de la fin comme vierges, afin nottament d'éviter les anomalies graphiques.
		map.set(startPos.x, startPos.y, false);
		map.set(endPos.x, endPos.y, false);

		// Ici, on change la couleur du cruseur sur la map, en fonction de ce que l'on place (début, fin, ou "bloc")
		if (settingStart)
			drawer.setColor(sf::Color(255, 128, 64, 128));
		else if (settingSearch)
			drawer.setColor(sf::Color(64, 255, 128, 128));
		else
			drawer.setColor(sf::Color(64, 128, 255, 128));

		// Ici, on fait le calcul du chemin, ainsi que la mesure du temps
		if (shouldUpdatePath) {
			sf::Clock clock;

			pathfinder.computePath(map, startPos.x, startPos.y, endPos.x, endPos.y);

			sf::Time time = clock.getElapsedTime();

			pathfinder.computePathGraphics();

			std::wstringstream wss;
			wss << L"Chemin calculé en " << time.asMicroseconds() << L"µs";

			console.addText(wss.str());

			pathInfo.setInformations(pathfinder.path().size(), time.asMicroseconds(), pathfinder.checkedCases());

			shouldUpdatePath=false;
		}

		// On met à jour les éléments effectuants des animations.
		console.update();
		map.update();

		// Le dessin commence ici.
		window.clear();

		map.draw(window);

		{ // C'est ici que
			sf::RectangleShape rect(sf::Vector2f(28, 28));

			for (int i=0; i<map.height(); ++i) {
				map.drawWallLayer(window, i);

				if (startPos.y == i) {
					rect.setPosition(startPos.x*32+2, startPos.y*32+2);
					rect.setFillColor(sf::Color(255, 128, 64, 255));
					window.draw(rect);
				}
				if (endPos.y == i) {
					rect.setPosition(endPos.x*32+2, endPos.y*32+2);
					rect.setFillColor(sf::Color(64, 255, 128, 255));
					window.draw(rect);
				}
			}
		}

		if (!pathfinder.path().empty())
			pathfinder.draw(window, showAdvancedPath);

		buttons.draw(window);
		pathInfo.draw(window);
		drawer.draw(window);
        console.draw(window);

		window.display();
	}

	return 0;
}
