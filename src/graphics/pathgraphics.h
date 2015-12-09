#ifndef PATHGRAPHICS_H
#define PATHGRAPHICS_H

#include <SFML/Graphics.hpp>
#include <list>

#include "../path/pathcontext.h"
#include "../util/vec2.h"

class PathGraphics {
public:
	PathGraphics();

	void setShowingNodes(bool doShow);
	bool isShowingNodes() const;

	void updatePath(Vec2i start, std::list<Direction>& path);
	void draw(sf::RenderWindow& window, PathContext* context);

private:
	sf::VertexArray mPath;
	sf::Color mPathColor;

	bool mShowNodes;
};

#endif // PATHGRAPHICS_H
