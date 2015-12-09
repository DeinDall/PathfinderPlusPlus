#include "pathfinder.h"
#include <algorithm>

const unsigned int PathFinder::msInfiniteStepCount(UINT_MAX);

PathFinder::PathFinder()
	: mContext(nullptr), mUseAStar(true) {}

PathFinder::~PathFinder() {
	if (mContext != nullptr)
		delete mContext;
}

Vec2i PathFinder::getStartPos() const {
	if (mContext==nullptr)
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
	if (mContext != nullptr)
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

	PathContext::Node* node = endNode;


	if (!node->previousDirection.isValid())
		node = getWantedUncheckedNode();
	if (node==nullptr)
		node = startNode;

	mPathEndNode = node;

	while (node != startNode) {
		mPath.push_front(node->previousDirection);
		mContext->getStats().pathLength += getDirectionStepLength(node->previousDirection);

		Direction opp = node->previousDirection.opposite();
		node = mContext->get(node->x+opp.xOffset, node->y+opp.yOffset);
	}

	mContext->getStats().pathMinLength = mContext->getStats().pathLength +
										 mContext->distancePredict(mPathEndNode, endNode);
}

PathContext::Node* PathFinder::getWantedUncheckedNode() {
	return (mUseAStar ? getBestUncheckedNode() : getCloserUncheckedNode());
}

PathContext::Node* PathFinder::getCloserUncheckedNode() {
	PathContext::Node* curNode = nullptr;

	for (PathContext::Node* node : mUncheckedNodes) {
		if (curNode==nullptr)
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
			continue;
		else if (!mContext->inMap(nodePos.x, nodePos.y))
			continue;
		else if (mContext->isWall(node->x, node->y, dir))
			continue;

		PathContext::Node* n = mContext->get(nodePos.x, nodePos.y);
		unsigned int stepValue = getDirectionStepLength(dir);

		if (node->stepCount+stepValue < n->stepCount) {
			if (!n->previousDirection.isValid()) // means has not been added yet
				mUncheckedNodes.push_back(n);

			n->stepCount = node->stepCount+stepValue;
			n->previousDirection = dir;
		}
	}

	mUncheckedNodes.remove(node);
	++mContext->getStats().checkedNodes;
}

unsigned int PathFinder::getDirectionStepLength(Direction dir) {
	return (dir.isCardinal() ? 100 : 144);
}
