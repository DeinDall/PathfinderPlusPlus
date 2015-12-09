#include "pathfinder.h"

Pathfinder::Pathfinder() {
	mPathGraphics.setPrimitiveType(sf::LinesStrip);
	mGraphicsColor = sf::Color(32, 128, 0, 128);
}

void Pathfinder::computePath(Map& map, int fromX, int fromY, int toX, int toY) {
	mOrigin = sf::Vector2i(fromX, fromY);
	mCheckedCases = 0;

	mStepCountMap.resize(map.width()*map.height());
	for (int& i : mStepCountMap)
		i = -1;
	mMapWidth = map.width();

	std::vector<Node> nodes;
	nodes.resize(map.width()*map.height());

	for (int ix=0; ix<map.width(); ++ix) {
		for (int iy=0; iy<map.height(); ++iy) {
			Node& node = nodes[positionToArrayIndex(ix, iy, map.width())];

			node.stepCount = -1;

			node.x = ix;
			node.y = iy;
		}
	}

	Node& start = nodes[positionToArrayIndex(fromX, fromY, map.width())];
	Node& end = nodes[positionToArrayIndex(toX, toY, map.width())];

	start.stepCount=0;

	std::list<Node*> remainingNodes;
	remainingNodes.push_back(&start);

	while (!remainingNodes.empty()) {
		Node* n1 = nullptr;

		// Ceci n'est pas part de l'algorithme, mais sert a suivre son fonctionnement
		mCheckedCases++;

		// On cherche le noeud ayant le chemin le plus court depuis le début
		for (Node* node : remainingNodes) {
			if (n1==nullptr)
				n1 = node;
			else if ((n1->stepCount + manhattan(n1->x, n1->y, end.x, end.y))>(node->stepCount + manhattan(node->x, node->y, end.x, end.y)))
				n1 = node;
		}

		mStepCountMap[positionToArrayIndex(n1->x, n1->y, map.width())] = n1->stepCount;

		// Si on tombe sur la fin, on a le chemin le plus court vers la fin.
		if (n1 == &end)
			break;

		// On enleve le noeud trouvé de la liste d'attente
		remainingNodes.remove(n1);

		// On parcourt les voisins de notre noeud
		for (Direction dir : Direction::validDirections) {
			int n2x = n1->x + dir.xOffset, n2y = n1->y+dir.yOffset;

			if (dir==n1->previousDirection.opposite())
				continue; // inutile, c'est la d'où on viens
			else if (n2x<0 || n2x>=map.width() || n2y<0 || n2y>=map.height())
				continue; // inutile, c'est hors de la map
			else if (map.get(n2x, n2y) != 0)
				continue; // inutile, c'est un bloc solide

			Node& n2 = nodes[positionToArrayIndex(n2x, n2y, map.width())];

			if ((n2.stepCount==-1) || (n2.stepCount > n1->stepCount+1) ) {
				n2.stepCount = n1->stepCount+1;
				n2.previousDirection = dir;
				remainingNodes.push_back(&n2);
			}
		}
	}

	std::vector<Direction> reversePath;

	Node* n = &end;

	if (!n->previousDirection.isValid()) {
		mPath.clear();
		return;
	}

	while (n != &start) {
		reversePath.push_back(n->previousDirection);

		Direction opp = n->previousDirection.opposite();
		n = &nodes[positionToArrayIndex(n->x + opp.xOffset, n->y + opp.yOffset, map.width())];
	}

	mPath.resize(reversePath.size());

	for (int i=0; i<mPath.size(); ++i) {
		mPath[i] = reversePath[mPath.size()-(i+1)];
	}
}

void Pathfinder::computePathGraphics() {
	mPathGraphics.resize(mPath.size()+1);
	sf::Vector2f currentPos(mOrigin.x, mOrigin.y);

	mPathGraphics[0] = sf::Vertex(sf::Vector2f(16, 16) + currentPos*32.f, mGraphicsColor);

	for (int i=0; i<mPath.size(); ++i) {
		currentPos.x += mPath[i].xOffset;
		currentPos.y += mPath[i].yOffset;

		mPathGraphics[i+1] = sf::Vertex(sf::Vector2f(16, 16) + currentPos*32.f, mGraphicsColor);
	}
}

void Pathfinder::draw(sf::RenderWindow& window, bool doStepCountMap) {
	glLineWidth(4.f);
	window.draw(mPathGraphics);
	glLineWidth(1.f);

	if (doStepCountMap) {
		sf::RectangleShape rect(sf::Vector2f(6, 6));

		for (int i=0; i<mStepCountMap.size(); ++i) {
			if (mStepCountMap[i]==-1)
				continue;
			int x = i%mMapWidth, y = i/mMapWidth;

			rect.setFillColor(sf::Color((mStepCountMap[i]/(float)mPath.size())*255, 64,
										255-(mStepCountMap[i]/(float)mPath.size())*255));
			rect.setPosition(x*32+13, y*32+13);
			window.draw(rect);
		}
	}
}

std::vector<Direction>& Pathfinder::path() {
	return mPath;
}

int Pathfinder::checkedCases() {
	return mCheckedCases;
}

bool Pathfinder::tilesAreConnected(int x1, int y1, int x2, int y2) {
	// deux tuiles sont connectées si la distance de manhattan entre ces deux tuiles est egales à 1.
	return (manhattan(x1, y1, x2, y2) == 1);
}

int Pathfinder::manhattan(int x1, int y1, int x2, int y2) {
	return std::abs(x1-x2)+std::abs(y1-y2);
}

int Pathfinder::positionToArrayIndex(int x, int y, int w) {
	return y*w+x;
}
