#include "flag.h"

Flag::Flag(std::string flagTexName) {
	mTexture.loadFromFile(sf::String(std::string("resources/") + flagTexName));
	mSprite.setTexture(mTexture);
	mImage = 0.f;
}

void Flag::setPosition(sf::Vector2f pos) {
	mSprite.setPosition(pos);
}

void Flag::update() {
	mImage += 0.2;

	if (mImage >= 8)
		mImage -= 8; // mieux que =0, car permet de garder le décalage si jamais il y a.

	setImage((int)mImage);
}

void Flag::draw(sf::RenderWindow& window) {
	window.draw(mSprite);
}

void Flag::setImage(int img) {
	mSprite.setTextureRect(sf::IntRect(img*32, 0, 32, 32));
}
