#include "pathgraphics.h"

#include <GL/gl.h>

PathGraphics::PathGraphics()
	: mShowNodes(false) {
	mPath.setPrimitiveType(sf::LinesStrip);
	mPathColor = sf::Color(32, 128, 0, 128);
}

void PathGraphics::setShowingNodes(bool doShow) {
	mShowNodes = doShow;
}

bool PathGraphics::isShowingNodes() const {
	return mShowNodes;
}

void PathGraphics::updatePath(Vec2i start, std::list<Direction>& path) {
	mPath.resize(path.size()+1);
	sf::Vector2f currentPos(start.x, start.y);

	mPath[0] = sf::Vertex(sf::Vector2f(16, 16) + currentPos*32.f, mPathColor);

	int i=0;
	for (Direction dir : path) {
		currentPos.x += dir.xOffset;
		currentPos.y += dir.yOffset;

		mPath[++i] = sf::Vertex(sf::Vector2f(16, 16) + currentPos*32.f, mPathColor);
	}
}

void PathGraphics::draw(sf::RenderWindow& window, PathContext* context) {
	if (context == nullptr)
		return;

	if (mPath.getVertexCount()>2) {
		glLineWidth(4.f);
		window.draw(mPath);
		glLineWidth(1.f);
	}

	if (mShowNodes) { // TODO: move node graphics generation elsewhere (like path)
		sf::RectangleShape rect(sf::Vector2f(6, 6));
		int gradiantGoal = context->getStats().pathMinLength;

		for (unsigned int ix=0; ix<context->width(); ++ix) {
			for (unsigned int iy=0; iy<context->height(); ++iy) {
				float sc = (context->get(ix, iy)->stepCount/(float)gradiantGoal);

				if (sc>1.f)
					continue;

				rect.setFillColor(sf::Color(sc*255, 128, ((1.f-sc)*255)));
				rect.setPosition(ix*32+13, iy*32+13);

				window.draw(rect);
			}
		}
	}
}
