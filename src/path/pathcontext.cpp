#include "pathcontext.h"
#include "../util/vec2.h"

PathContext::PathContext(const Map& map, int startX, int startY, int endX, int endY)
	: mWidth(map.width()), mHeight(map.height()) {
	mNodes.resize(mWidth*mHeight);
	mWalls.resize(mWidth*mHeight);

	for (int ix=0; ix<mWidth; ++ix) {
		for (int iy=0; iy<mHeight; ++iy) {
			mWalls[ix+iy*mWidth] = map.get(ix, iy)!=0;
			Node& node = mNodes[ix+iy*mWidth];

			node.stepCount = UINT_MAX;

			node.x = ix;
			node.y = iy;
		}
	}

	mStartNode = get(startX, startY);
	mEndNode = get(endX, endY);

	mStartNode->stepCount = 0;

	mStats.checkedNodes = 0;
	mStats.pathLength = 0;
	mStats.pathMinLength = 0;
}

PathContext::Node* PathContext::get(unsigned int x, unsigned int y) {
	return &mNodes[x+y*mWidth];
}

PathContext::Node* PathContext::getStart() {
	return mStartNode;
}

PathContext::Node* PathContext::getEnd() {
	return mEndNode;
}

PathContext::Stats& PathContext::getStats() {
	return mStats;
}

bool PathContext::isWall(unsigned int x, unsigned int y, Direction dir) const {
	Vec2i v(x+dir.xOffset, y+dir.yOffset);
	return mWalls[v.x+v.y*mWidth] || mWalls[v.x+y*mWidth] || mWalls[x+v.y*mWidth];
}

bool PathContext::inMap(unsigned int x, unsigned int y) const {
	return !(x<0 || y<0 || x>=mWidth || y>=mHeight);
}

unsigned int PathContext::distancePredict(Node* first, Node* second) const {
	// manhattan distance
	return (std::abs(first->x-second->x)+std::abs(first->y-second->y))*100;
}

unsigned int PathContext::width() const {
	return mWidth;
}

unsigned int PathContext::height() const {
	return mHeight;
}

