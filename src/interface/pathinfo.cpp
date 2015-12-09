#include "pathinfo.h"
#include <sstream>

PathInfo::PathInfo(sf::Font& font, sf::Vector2i pos) {
	mText.setFont(font);
	mText.setCharacterSize(18);
	mText.setPosition(pos.x, pos.y);
}

void PathInfo::setInformations(int pathSize, long long time, int cCount) {
	std::wstringstream wss;

	wss << L"\tInformations:" << std::endl << std::endl
		<< L"Longueur: " << pathSize << std::endl
		<< L"Temps de calcul: ";

	if (time!=0)
		wss << time << L"µs";
	else
		wss << L"?";

	wss << std::endl << L"Cases vérifiées: " << cCount;

	mText.setString(sf::String(wss.str()));
}

void PathInfo::draw(sf::RenderWindow& window) {
	window.draw(mText);
}
