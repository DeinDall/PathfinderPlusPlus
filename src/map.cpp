#include "map.h"

#include <fstream>

#define sign(x) (x>0 ? 1 : -1)

Map::Map(int w, int h) {
	mWidth = w;
	mHeight = h;

	mTexture.loadFromFile("resources/blocks.png");

	init();
}

Map::~Map() {}

int Map::get(int x, int y) const {
	return mData[y*mWidth+x].drawVal;
}

void Map::set(int x, int y, bool val) {
	mData[y*mWidth+x].val = val;
}

int Map::width() const {
	return mWidth;
}

int Map::height() const {
	return mHeight;
}

void Map::clear() {
	for (unsigned int i=0; i<mData.size(); ++i) {
		mData[i].val = false;
	}
}

void Map::update() {
	//*
	for (unsigned int i=0; i<mData.size(); ++i) {
		if (mData[i].drawVal!=(mData[i].val*8))
			mData[i].drawVal -= sign(mData[i].drawVal-(mData[i].val*8));
	} //*/
}

void Map::draw(sf::RenderWindow& window) {
	static sf::IntRect texRectGround(0, 0, 32, 32);

	sf::Sprite spr;
	spr.setTexture(mTexture);
	spr.setTextureRect(texRectGround);

	// dessin du sol
	for (int ix=0; ix<width(); ++ix) {
		for (int iy=0; iy<height(); ++iy) {
			spr.setPosition(ix*32, iy*32);
			window.draw(spr);
		}
	}
}

void Map::drawWallLayer(sf::RenderWindow& window, int yLayer) {
	static sf::IntRect texRectGround(0, 0, 32, 32), texRectWall(32, 0, 32, 32);

	sf::Sprite spr;
	spr.setTexture(mTexture);

	int yOffset=0;
	sf::Color color(255, 255, 255, 0);

	for (int ix=0; ix<width(); ++ix) {
		if (get(ix, yLayer)==0)
			continue;

		yOffset = -get(ix, yLayer);
		color.a = (get(ix, yLayer)/8.f*255);

		spr.setColor(sf::Color::White);

		// On dessine la partie latérale du mur
		spr.setTextureRect(sf::IntRect(64, 0, 32, -yOffset));
		spr.setPosition(ix*32, (yLayer+1)*32+yOffset);
		window.draw(spr);

		spr.setPosition(ix*32, yLayer*32+yOffset);

		// on dessine un sol sur le mur
		spr.setTextureRect(texRectGround);
		window.draw(spr);

		// on dessine un sommet de mur, avec une opacitée dépendant de la hauteur du mur (pour l'animation)
		spr.setTextureRect(texRectWall);
		spr.setColor(color);
		window.draw(spr);
	}
}

void Map::save(std::string file) {
	std::ofstream ffile;
	ffile.open(sf::String(file).toAnsiString(), std::ofstream::binary | std::ofstream::trunc);

	ffile.write(reinterpret_cast<const char*>(&mWidth), sizeof(int));
	ffile.write(reinterpret_cast<const char*>(&mHeight), sizeof(int));
	ffile.write(reinterpret_cast<const char*>(&mData[0]), sizeof(tile)*mData.size());

	ffile.close();
}

void Map::load(std::string file) {
	std::ifstream ffile;
	ffile.open(sf::String(file).toAnsiString(), std::ifstream::binary);

	ffile.read(reinterpret_cast<char*>(&mWidth), sizeof(int));
	ffile.read(reinterpret_cast<char*>(&mHeight), sizeof(int));

	mData.resize(mWidth*mHeight);
	ffile.read(reinterpret_cast<char*>(&mData[0]), sizeof(tile)*mData.size());

	ffile.close();
}

void Map::init() {
	mData.resize(mWidth*mHeight);
	for (unsigned int i=0; i<mData.size(); ++i) {
		mData[i].drawVal = 0;
		mData[i].val = false;
	}
}
