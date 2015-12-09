#ifndef DIRECTION_H
#define DIRECTION_H

class Direction {
public:
	static Direction UP, DOWN, LEFT, RIGHT;
	static Direction validDirections[4];

	Direction();
	Direction opposite();
	bool isValid();

private:
	// constructeurs privés
	Direction(int xo, int yo) : xOffset(xo), yOffset(yo) {}

public:
	int xOffset, yOffset;
};

bool operator==(const Direction& d1, const Direction& d2);

#endif // DIRECTION_H
