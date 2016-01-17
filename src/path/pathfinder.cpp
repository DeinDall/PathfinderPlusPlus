#include "pathfinder.h"
#include <algorithm>

const unsigned int PathFinder::msInfiniteStepCount(UINT_MAX);

PathFinder::PathFinder()
	: mContext(nullptr), mUseAStar(true) {}

PathFinder::~PathFinder() {
	if (mContext)
		delete mContext;
}

Vec2i PathFinder::getStartPos() const {
	if (!mContext)
		return Vec2i(-1, -1);

	PathContext::Node* node = mContext->getStart();
	return Vec2i(node->x, node->y);
}

std::list<Direction>& PathFinder::getPath() {
	return mPath;
}

PathContext* PathFinder::getContext() const {
	return mContext;
}

PathContext::Stats PathFinder::getStats() const {
	return mContext->getStats();
}

void PathFinder::computePath(PathContext* context) {
	startPathfind(context);

	while (shouldContinue())
		forwardPathfind();

	generatePath();
}

void PathFinder::startPathfind(PathContext* context) {
	if (mContext)
		delete mContext;
	mContext = context;

	mPath.clear();

	mUncheckedNodes.clear();
	mUncheckedNodes.push_back(mContext->getStart());
}

bool PathFinder::shouldContinue() const {
	return canContinue() && !mContext->getEnd()->previousDirection.isValid();
}

bool PathFinder::canContinue() const {
	return !mUncheckedNodes.empty();
}

void PathFinder::forwardPathfind() {
	checkNode(getWantedUncheckedNode());
}

void PathFinder::generatePath() {
	mPath.clear();
	mContext->getStats().pathLength=0;

	PathContext::Node* startNode = mContext->getStart();
	PathContext::Node* endNode = mContext->getEnd();

	// We build the path by iterating through the nodes backwards.
	// That is why the first pointed node is the end node.

	PathContext::Node* node = endNode;

	// If we didn't find the end node, we will build the path from the
	// next node to check.
	if (!node->previousDirection.isValid())
		node = getWantedUncheckedNode();

	if (!node)
		node = startNode;

	mPathEndNode = node;

	// Once we have the last node, we start iterating.
	while (node != startNode) {
		mPath.push_front(node->previousDirection);
		mContext->getStats().pathLength += getDirectionStepLength(node->previousDirection);

		Direction opp = node->previousDirection.opposite();
		node = mContext->get(node->x+opp.xOffset, node->y+opp.yOffset);
	}

	// This computes the minimal distance the path is to the end node.
	mContext->getStats().pathMinLength = mContext->getStats().pathLength +
										 mContext->distancePredict(mPathEndNode, endNode);
}

PathContext::Node* PathFinder::getWantedUncheckedNode() {
	// This is the only thing that changes between A* and Dijkstra.
	return (mUseAStar ? getBestUncheckedNode() : getCloserUncheckedNode());
}

PathContext::Node* PathFinder::getCloserUncheckedNode() {
	PathContext::Node* curNode = nullptr;

	for (PathContext::Node* node : mUncheckedNodes) {
		if (!curNode)
			curNode = node;
		else if (node->stepCount < curNode->stepCount)
			curNode = node;
	}

	return curNode;
}

PathContext::Node* PathFinder::getBestUncheckedNode() {
	PathContext::Node* curNode = nullptr;
	PathContext::Node* endNode = mContext->getEnd();

	for (PathContext::Node* node : mUncheckedNodes) {
		if (curNode==nullptr)
			curNode = node;
		else if ((node->stepCount+mContext->distancePredict(node, endNode)) < (curNode->stepCount+mContext->distancePredict(curNode, endNode)))
			curNode = node;
	}

	return curNode;
}

void PathFinder::checkNode(PathContext::Node* node) {
	for (Direction dir : Direction::validDirections) {
		Vec2i nodePos(node->x+dir.xOffset, node->y+dir.yOffset);

		if (dir.opposite()==node->previousDirection)
			continue; // This node is where we came from.
		else if (!mContext->inMap(nodePos.x, nodePos.y))
			continue; // This node does not exist (out of bounds).
		else if (mContext->isWall(node->x, node->y, dir))
			continue; // This direction is not valid for a path (wall).

		PathContext::Node* n = mContext->get(nodePos.x, nodePos.y);
		unsigned int stepValue = getDirectionStepLength(dir);

		// We will change the node if it is worth it (the new path is shorter).
		// Note that the starting value of stepCount is UINT_MAX (close to infinity).
		if (node->stepCount+stepValue < n->stepCount) {
			if (!n->previousDirection.isValid()) // means has not been added yet.
				mUncheckedNodes.push_back(n);

			n->stepCount = node->stepCount+stepValue;
			n->previousDirection = dir;
		}
	}

	mUncheckedNodes.remove(node);
	++mContext->getStats().checkedNodes;
}

unsigned int PathFinder::getDirectionStepLength(Direction dir) {
	// 141 ~= 100*sqrt(2), the distance for a diagonal segment.
	return (dir.isCardinal() ? 100 : 141);
}
