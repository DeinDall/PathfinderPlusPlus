#include "map.h"

Map::Map(int w, int h) {
	mWidth = w;
	mHeight = h;

	mTexture.loadFromFile("resources/blocks.png");

	init();
}

int Map::get(int x, int y) {
	return mData[y*mWidth+x];
}

void Map::set(int x, int y, int val) {
	mData[y*mWidth+x] = val;
}

int Map::width() {
	return mWidth;
}

int Map::height() {
	return mHeight;
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

		spr.setTextureRect(sf::IntRect(64, 0, 32, -yOffset));
		spr.setPosition(ix*32, (yLayer+1)*32+yOffset);
		window.draw(spr);

		spr.setPosition(ix*32, yLayer*32+yOffset);

		spr.setTextureRect(texRectGround);
		window.draw(spr);

		spr.setTextureRect(texRectWall);
		spr.setColor(color);
		window.draw(spr);
	}
}

void Map::save() {
	std::ofstream file;
	file.open("pathfindermap.srpfmp", std::ofstream::binary | std::ofstream::trunc);

	file.write(reinterpret_cast<const char*>(&mWidth), sizeof(int));
	file.write(reinterpret_cast<const char*>(&mHeight), sizeof(int));
	file.write(reinterpret_cast<const char*>(&mData[0]), sizeof(int)*mData.size());

	file.close();
}

void Map::load(std::string file) {
	std::ifstream ffile;
	ffile.open(file, std::ifstream::binary);

	ffile.read(reinterpret_cast<char*>(&mWidth), sizeof(int));
	ffile.read(reinterpret_cast<char*>(&mHeight), sizeof(int));

	mData.resize(mWidth*mHeight);
	ffile.read(reinterpret_cast<char*>(&mData[0]), sizeof(int)*mData.size());

	ffile.close();
}

void Map::init() {
	mData.resize(mWidth*mHeight);
	for (unsigned int i=0; i<mData.size(); ++i)
		mData[i] = 0;
}
