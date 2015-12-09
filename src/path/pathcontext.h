#ifndef PATHCONTEXT_H
#define PATHCONTEXT_H

#include "../map.h"
#include "../util/direction.h"

class PathContext {
public:
	struct Node {
		int x, y;
		unsigned int stepCount;
		Direction previousDirection;
	};

	struct Stats {
		unsigned int checkedNodes;
		unsigned int pathLength;
		unsigned int pathMinLength;
	};

public:
	PathContext(const Map& map, int startX, int startY, int endX, int endY);

	Node* get(unsigned int x, unsigned int y);
	Node* getStart();
	Node* getEnd();

	Stats& getStats();

	bool isWall(unsigned int x, unsigned int y, Direction dir) const;
	bool inMap(unsigned int x, unsigned int y) const;

	unsigned int distancePredict(Node* first, Node* second) const;

	unsigned int width() const;
	unsigned int height() const;

private:
	std::vector<Node> mNodes;
	std::vector<bool> mWalls;
	unsigned int mWidth, mHeight;

	Node* mStartNode;
	Node* mEndNode;

	Stats mStats;
};

#endif // PATHCONTEXT_H
