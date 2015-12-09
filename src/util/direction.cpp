#include "direction.h"

Direction Direction::UP(0, -1);
Direction Direction::DOWN(0, 1);
Direction Direction::LEFT(-1, 0);
Direction Direction::RIGHT(1, 0);

Direction Direction::validDirections[4] = { Direction::RIGHT, Direction::UP, Direction::LEFT, Direction::DOWN };

Direction::Direction()
	: Direction(0, 0) {}

Direction Direction::opposite() const {
	return Direction(-xOffset, -yOffset);
}

bool Direction::isValid() const {
	return ((xOffset!=0) || (yOffset!=0));
}

Direction::Direction(int xo, int yo)
	: xOffset(xo), yOffset(yo) {}

bool operator==(const Direction& d1, const Direction& d2) {
	return ((d1.xOffset==d2.xOffset) && (d1.yOffset==d2.yOffset));
}

bool operator!=(const Direction& d1, const Direction& d2) {
	return !(d1==d2);
}
