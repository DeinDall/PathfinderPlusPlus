#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <list>

#include "../util/vec2.h"
#include "pathcontext.h"

class PathFinder {
public:
	PathFinder();
	~PathFinder();

	Vec2i getStartPos() const;
	std::list<Direction>& getPath();

	PathContext* getContext() const;
	PathContext::Stats getStats() const;

	void computePath(PathContext* context);

	void startPathfind(PathContext* context);

	bool shouldContinue() const;
	bool canContinue() const;

	void forwardPathfind();
	void generatePath();

protected:
	PathContext::Node* getWantedUncheckedNode();
	PathContext::Node* getCloserUncheckedNode();
	PathContext::Node* getBestUncheckedNode();
	void checkNode(PathContext::Node* node);

private:

	unsigned int getDirectionStepLength(Direction dir);

private:
	bool mUseAStar;

	PathContext* mContext;
	std::list<Direction> mPath;

	std::list<PathContext::Node*> mUncheckedNodes;
	PathContext::Node* mPathEndNode;

public:
	static const unsigned int msInfiniteStepCount;
};

#endif // PATHFINDER_H
