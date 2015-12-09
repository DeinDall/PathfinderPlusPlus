#include "pathfinder.h"
#include <algorithm>

const unsigned int Pathfinder::msInvalidStepCount(UINT_MAX);

Pathfinder::NodeMap::NodeMap(const Map& map) : mWidth(map.width()), mHeight(map.height()) {
	mNodes.resize(mWidth*mHeight);
	mWalls.resize(mWidth*mHeight);

	for (int ix=0; ix<mWidth; ++ix) {
		for (int iy=0; iy<mHeight; ++iy) {
			mWalls[ix+iy*mWidth] = map.get(ix, iy)!=0;
			Node& node = mNodes[ix+iy*mWidth];

			node.stepCount = msInvalidStepCount;

			node.x = ix;
			node.y = iy;
		}
	}
}

Pathfinder::Node* Pathfinder::NodeMap::get(unsigned int x, unsigned int y) {
	return &mNodes[x+y*mWidth];
}

bool Pathfinder::NodeMap::isWall(unsigned int x, unsigned int y) const {
	return mWalls[x+y*mWidth];
}

bool Pathfinder::NodeMap::inMap(unsigned int x, unsigned int y) const {
	return !(x<0 || y<0 || x>=mWidth || y>=mHeight);
}

unsigned int Pathfinder::NodeMap::width() const {
	return mWidth;
}

unsigned int Pathfinder::NodeMap::height() const {
	return mHeight;
}

Pathfinder::Pathfinder() : mStarted(false), mUseAStar(true) {
	mPathGraphics.setPrimitiveType(sf::LinesStrip);
	mGraphicsColor = sf::Color(32, 128, 0, 128);
}

Pathfinder::~Pathfinder() {
	if (mStarted)
		delete mNodeMap;
}

void Pathfinder::computePath(Map& map, int fromX, int fromY, int toX, int toY) {
	startPathfind(map, fromX, fromY, toX, toY);

	while (!pathfindFinished())
		forwardPathfind();

	generatePath();
}

void Pathfinder::computePathGraphics() {
	mPathGraphics.resize(mPath.size()+1);
	sf::Vector2f currentPos(mStartNode->x, mStartNode->y);

	mPathGraphics[0] = sf::Vertex(sf::Vector2f(16, 16) + currentPos*32.f, mGraphicsColor);

	int i=0;
	for (Direction dir : mPath) {
		currentPos.x += dir.xOffset;
		currentPos.y += dir.yOffset;

		mPathGraphics[++i] = sf::Vertex(sf::Vector2f(16, 16) + currentPos*32.f, mGraphicsColor);
	}
}

void Pathfinder::draw(sf::RenderWindow& window, bool doStepCountMap) {
	if (mPathGraphics.getVertexCount()>2) {
		// je vous présente un petit hack de la SFML au travers de OpenGL.
		glLineWidth(4.f);
		window.draw(mPathGraphics);
		glLineWidth(1.f);
	}

	// Ici on dessine les cases visitées dans une couleure dépendante de leur distance en pas de l'origine
	if (doStepCountMap && mStarted) {
		sf::RectangleShape rect(sf::Vector2f(6, 6));
		int gradiantGoal = mPath.size()+manhattan(mPathEndNode, mEndNode);

		// On parcours la carte des nombres de pas
		for (unsigned int ix=0; ix<mNodeMap->width(); ++ix) {
			for (unsigned int iy=0; iy<mNodeMap->height(); ++iy) {
				unsigned int sc = mNodeMap->get(ix, iy)->stepCount;
				if (sc==msInvalidStepCount)
					continue; // on affiche pas les cases non visitées

				// Ceci est un dégradé Bleu/Cyan -> Rouge/Orange
				rect.setFillColor(sf::Color((sc/(float)gradiantGoal)*255, 64,
											255-(sc/(float)gradiantGoal)*255));

				// On dessine à la position voulue
				rect.setPosition(ix*32+13, iy*32+13);
				window.draw(rect);
			}

		}
	}
}

std::list<Direction>& Pathfinder::path() {
	return mPath;
}

unsigned int Pathfinder::checkedNodes() const {
	return mStatCheckedNodes;
}

void Pathfinder::startPathfind(Map& map, int fromX, int fromY, int toX, int toY) {
	resetNodeMap(map);

	mStatCheckedNodes=0;

	mPath.clear();

	mStartNode = mNodeMap->get(fromX, fromY);
	mEndNode = mNodeMap->get(toX, toY);

	mStartNode->stepCount=0;

	mUncheckedNodes.clear();
	mUncheckedNodes.push_back(mStartNode);

	computePathGraphics();
}

bool Pathfinder::pathfindFinished() const {
	return mUncheckedNodes.empty() || mEndNode->previousDirection.isValid();
}

void Pathfinder::forwardPathfind() {
	checkNode(getWantedUncheckedNode());
}

void Pathfinder::generatePath() {
	mPath.clear();

	Node* node = mEndNode;

	if (!node->previousDirection.isValid())
		node = getWantedUncheckedNode();
	if (node==nullptr)
		node = mStartNode;

	mPathEndNode = node;

	while (node != mStartNode) {
		mPath.push_front(node->previousDirection);

		Direction opp = node->previousDirection.opposite();
		node = mNodeMap->get(node->x+opp.xOffset, node->y+opp.yOffset);
	}
}

Pathfinder::Node* Pathfinder::getWantedUncheckedNode() {
	return (mUseAStar ? getBestUncheckedNode() : getCloserUncheckedNode());
}

Pathfinder::Node* Pathfinder::getCloserUncheckedNode() {
	Node* curNode = nullptr;

	for (Node* node : mUncheckedNodes) {
		if (curNode==nullptr)
			curNode = node;
		else if (node->stepCount < curNode->stepCount)
			curNode = node;
	}

	return curNode;
}

Pathfinder::Node* Pathfinder::getBestUncheckedNode() {
	Node* curNode = nullptr;

	for (Node* node : mUncheckedNodes) {
		if (curNode==nullptr)
			curNode = node;
		else if ((node->stepCount+manhattan(node, mEndNode)) < (curNode->stepCount+manhattan(curNode, mEndNode)))
			curNode = node;
	}

	return curNode;
}

void Pathfinder::checkNode(Node* node) {
	for (Direction dir : Direction::validDirections) {
		sf::Vector2i nodePos(node->x+dir.xOffset, node->y+dir.yOffset);

		if (dir.opposite()==node->previousDirection)
			continue;
		else if (!mNodeMap->inMap(nodePos.x, nodePos.y))
			continue;
		else if (mNodeMap->isWall(nodePos.x, nodePos.y))
			continue;

		Node* n = mNodeMap->get(nodePos.x, nodePos.y);

		if (node->stepCount+1 < n->stepCount) {
			if (!n->previousDirection.isValid()) // means has not been added yet
				mUncheckedNodes.push_back(n);

			n->stepCount = node->stepCount+1;
			n->previousDirection = dir;
		}
	}

	mUncheckedNodes.remove(node);
	++mStatCheckedNodes;
}

void Pathfinder::resetNodeMap(const Map& map) {
	freeNodeMap();
	mNodeMap = new NodeMap(map);
	mStarted = true;
}

void Pathfinder::freeNodeMap() {
	if (mStarted)
		delete mNodeMap;
}

int Pathfinder::manhattan(Node* one, Node* two) {
	return std::abs(one->x-two->x)+std::abs(one->y-two->y);
}
