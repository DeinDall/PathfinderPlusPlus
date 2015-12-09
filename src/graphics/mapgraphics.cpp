#include "mapgraphics.h"

#define sign(x) ((x)>0 ? 1 : -1)

MapGraphics::MapGraphics() {
	initRenderTexture();
}

void MapGraphics::update(Map& map) {
	mWidth = map.width();
	mHeight = map.height();

	if (mData.size() != mWidth*mHeight)
		mData.resize(mWidth*mHeight);

	for (unsigned int i=0; i<mData.size(); ++i) {
		unsigned int x = i%mWidth, y = i/mWidth;
		int goal = map.get(x, y)*8;

		if (mData[i]!=goal) {
			mData[i] -= sign(mData[i]-goal);
			mLayerNeedsUpdate[y] = true;
		}
	}

	if (mHeight != mLayers.size()) {
		mLayers.resize(mHeight);
		mLayerNeedsUpdate.resize(mHeight);

		for (sf::VertexArray& array : mLayers) {
			array.setPrimitiveType(sf::Triangles);
			array.resize(mWidth*6);
		}

		for (unsigned int i=0; i<mHeight; ++i)
			mLayerNeedsUpdate[i]=true;
	}
}

void MapGraphics::drawWallLayer(sf::RenderTarget& window, int yLayer) {
	if (mLayerNeedsUpdate[yLayer])
		updateGraphics(yLayer);
	window.draw(mLayers[yLayer], &mRenderTexture.getTexture());//*/
//	window.draw(sf::Sprite(mRenderTexture.getTexture()));
}

void MapGraphics::initRenderTexture() {
	sf::Texture tex;
	tex.loadFromFile("resources/blocks.png");

	const sf::IntRect texRectGround(0, 0, 32, 32), texRectWall(32, 0, 32, 32);

	sf::Sprite spr;
	spr.setTexture(tex);
	spr.setPosition(0, 32);
	spr.setTextureRect(texRectGround);

	mRenderTexture.create(9*32, 64);
	mRenderTexture.draw(spr);

	int yOffset=0;
	sf::Color color(255, 255, 255, 0);

	for (int i=0; i<8; ++i) {
		yOffset = -(i+1);
		color.a = ((i+1)/8.f*255);

		spr.setColor(sf::Color::White);

		// On dessine la partie latérale du mur
		spr.setTextureRect(sf::IntRect(64, 0, 32, i+1));
		spr.setPosition((i+1)*32, 64+yOffset);
		mRenderTexture.draw(spr);

		spr.setPosition((i+1)*32, 32+yOffset);

		// on dessine un sol sur le mur
		spr.setTextureRect(texRectGround);
		mRenderTexture.draw(spr);

		// on dessine le sol plus sombre
		spr.setTextureRect(texRectWall);
		spr.setColor(color);
		mRenderTexture.draw(spr);
	}

	mRenderTexture.display();
}

void MapGraphics::updateGraphics(unsigned int layer) {
	static const sf::Vector2f offset[4] = {
		sf::Vector2f(0, -32),
		sf::Vector2f(0, 32),
		sf::Vector2f(32, -32),
		sf::Vector2f(32, 32)
	};

	sf::VertexArray& array = mLayers[layer];

	for (unsigned int ix=0; ix<mWidth; ++ix) {
		sf::Vector2f pos(ix*32, layer*32), tex(mData[ix+layer*mWidth]*32, 32);

		array[ix*6+0] = sf::Vertex(pos+offset[0], tex+offset[0]);
		array[ix*6+1] = sf::Vertex(pos+offset[1], tex+offset[1]);
		array[ix*6+2] = sf::Vertex(pos+offset[2], tex+offset[2]);

		array[ix*6+3] = sf::Vertex(pos+offset[1], tex+offset[1]);
		array[ix*6+4] = sf::Vertex(pos+offset[2], tex+offset[2]);
		array[ix*6+5] = sf::Vertex(pos+offset[3], tex+offset[3]);
	}
}
