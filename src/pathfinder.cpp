#include "pathfinder.h"

Pathfinder::Pathfinder() {}

void Pathfinder::computePath(Map& map, int fromX, int fromY, int toX, int toY) {
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

	/*
	for (Direction dir : Direction::validDirections) {
		if (map.get(fromX+dir.xOffset, fromY+dir.yOffset) == 0) {
			Node& n = nodes[positionToArrayIndex(fromX+dir.xOffset, fromY+dir.yOffset, map.width())];
			n.stepCount = 1;
			n.previousDirection = dir;
			remainingNodes.push_back(&n);
		}
	} //*/

	while (!remainingNodes.empty()) {
		Node* n1 = nullptr;

		// On cherche le noeud ayant le chemin le plus court depuis le début
		for (Node* node : remainingNodes) {
			if (n1==nullptr)
				n1 = node;
			else if ((n1->stepCount)>(node->stepCount))
				n1 = node;
		}

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

	if (!n->previousDirection.isValid())
		return;

	std::cout << "here" << std::endl;

	while (n != &start) {
		reversePath.push_back(n->previousDirection);

		Direction opp = n->previousDirection.opposite();
		n = &nodes[positionToArrayIndex(n->x + opp.xOffset, n->y + opp.yOffset, map.width())];
	}

	mPath.resize(reversePath.size());

	for (int i=0; i<mPath.size(); ++i) {
		mPath[i] = reversePath[mPath.size()-(i+1)];
		std::cout << mPath[i].xOffset << ";" << mPath[i].yOffset << std::endl;
	}
}

std::vector<Direction>& Pathfinder::path() {
	return mPath;
}

bool Pathfinder::tilesAreConnected(int x1, int y1, int x2, int y2) {
	// deux tuiles sont connectées si la distance de manhattan entre ces deux tuiles est egales à 1.
	return ((std::abs(x1-x2)+std::abs(y1-y2)) == 1);
}

int Pathfinder::positionToArrayIndex(int x, int y, int w) {
	return y*w+x;
}
