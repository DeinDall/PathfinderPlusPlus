#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "map.h"
#include "util/direction.h"

#include <GL/gl.h>
#include <list>

// classe calculant le chemin le plus court à l'aide de l'algorithme de Dijkstra/A*
class Pathfinder {
	struct Node {
		int x, y;
		unsigned int stepCount;
		Direction previousDirection;
	};

	class NodeMap {
	public:
		NodeMap(const Map& map);
		Node* get(unsigned int x, unsigned int y);
		bool isWall(unsigned int x, unsigned int y) const;
		bool inMap(unsigned int x, unsigned int y) const;
		unsigned int width() const;
		unsigned int height() const;

	private:
		std::vector<Node> mNodes;
		std::vector<bool> mWalls;
		unsigned int mWidth, mHeight;
	};

public:
	Pathfinder();
	~Pathfinder();

	void computePathGraphics();
	void draw(sf::RenderWindow& window, bool doStepCountMap);

	std::list<Direction>& path();

	unsigned int checkedNodes() const;

	void computePath(Map& map, int fromX, int fromY, int toX, int toY);

	void startPathfind(Map& map, int fromX, int fromY, int toX, int toY);
	bool pathfindFinished() const;
	void forwardPathfind();
	void generatePath();

protected:
	Node* getWantedUncheckedNode();
	Node* getCloserUncheckedNode();
	Node* getBestUncheckedNode();
	void checkNode(Node* node);

private:
	void resetNodeMap(const Map& map);
	void freeNodeMap();

	int manhattan(Node* one, Node* two);

private:
	sf::VertexArray mPathGraphics;
	sf::Vector2i mOrigin;
	sf::Color mGraphicsColor;

	bool mUseAStar;
	bool mStarted;

	std::list<Direction> mPath;

	NodeMap* mNodeMap;
	std::list<Node*> mUncheckedNodes;
	Node* mStartNode;
	Node* mEndNode;
	Node* mPathEndNode;

	unsigned int mStatCheckedNodes;

public:
	static const unsigned int msInvalidStepCount;
};

#endif // PATHFINDER_H
