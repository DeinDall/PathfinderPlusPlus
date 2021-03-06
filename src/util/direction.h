#ifndef DIRECTION_H
#define DIRECTION_H

class Direction {
public:
	static Direction UP, DOWN, LEFT, RIGHT;
	static Direction validDirections[8];

	Direction();
	Direction opposite() const;
	bool isValid() const;

	bool isCardinal() const;

private:
	// Private constructor.
	Direction(int xo, int yo);

	static Direction UPRIGHT, UPLEFT, DOWNLEFT, DOWNRIGHT;

public:
	int xOffset, yOffset;
};

bool operator==(const Direction& d1, const Direction& d2);
bool operator!=(const Direction& d1, const Direction& d2);

#endif // DIRECTION_H
