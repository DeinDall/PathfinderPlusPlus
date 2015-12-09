#include "map.h"

Map::Map(int w, int h) {
	mWidth = w;
	mHeight = h;

	init();
}

int Map::get(int x, int y) {
	return mData[y*mWidth+x];
}

void Map::set(int x, int y, int val) {
	mData[y*mWidth+x] = val;
}

int Map::width() {
	return mWidth;
}

int Map::height() {
	return mHeight;
}

void Map::init() {
	mData.resize(mWidth*mHeight);
	for (unsigned int i=0; i<mData.size(); ++i)
		mData[i] = 0;
}
