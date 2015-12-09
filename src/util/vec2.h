#ifndef VEC2_H
#define VEC2_H

template<typename T>
struct Vec2 {
public:
	Vec2() : Vec2(0, 0) {}
	Vec2(T xx, T yy) : x(xx), y(yy) {}

	T x, y;
};

typedef Vec2<int> Vec2i;

#endif // VEC2_H
