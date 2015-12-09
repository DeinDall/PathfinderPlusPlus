#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"
#include "direction.h"

#include <GL/gl.h>
#include <list>

// classe calculant le chemin le plus court à l'aide de l'algorithme de Dijkstra/A*
class Pathfinder {
	struct Node {
		int x, y;
		int stepCount;
		Direction previousDirection;
	};

public:
	Pathfinder();

	void computePath(Map& map, int fromX, int fromY, int toX, int toY);

	void computePathGraphics();
	void draw(sf::RenderWindow& window, bool doStepCountMap);

	std::vector<Direction>& path();

	int checkedCases();

protected:
	// NOTE: cette fonction est inutilisée
	bool tilesAreConnected(int x1, int y1, int x2, int y2);

	int manhattan(int x1, int y1, int x2, int y2);

	int positionToArrayIndex(int x, int y, int w);

private:
	// Le chemin: un tableau de Direction où chaque "pas" est représenté par une Direction.
	std::vector<Direction> mPath;
	sf::VertexArray mPathGraphics;
	sf::Vector2i mOrigin;
	sf::Color mGraphicsColor;

	int mMapWidth;
	std::vector<int> mStepCountMap;
};

#endif // PATHFINDER_H
