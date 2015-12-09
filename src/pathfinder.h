#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"
#include "direction.h"

#include <list>
#include <cstdlib>
#include <iostream>

// classe calculant le chemin le plus court à l'aide de l'algorithme de Dijkstra
class Pathfinder {
	struct Node {
		int x, y;
		int stepCount;
		Direction previousDirection;
	};

public:
	Pathfinder();

	void computePath(Map& map, int fromX, int fromY, int toX, int toY);
	std::vector<Direction>& path();

protected:
	bool tilesAreConnected(int x1, int y1, int x2, int y2);

	int positionToArrayIndex(int x, int y, int w);

private:
	// Le chemin: un tableau de Direction où chaque "pas" est représenté par une Direction.
	std::vector<Direction> mPath;
};

#endif // PATHFINDER_H
