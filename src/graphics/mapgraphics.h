#ifndef MAPGRAPHICS_H
#define MAPGRAPHICS_H

#include <SFML/Graphics.hpp>
#include "../map.h"

class MapGraphics {
public:
	MapGraphics();

	void update(Map& map);
	void drawWallLayer(sf::RenderTarget& target, int yLayer);

private:
	void initRenderTexture();
	void updateGraphics(unsigned int layer);

private:
	sf::RenderTexture mRenderTexture;
	std::vector<sf::VertexArray> mLayers;
	std::vector<bool> mLayerNeedsUpdate;

	std::vector<short> mData;

	unsigned int mWidth, mHeight;
};

#endif // MAPGRAPHICS_H
