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

void Map::save() {
	std::ofstream file;
	file.open("pathfindermap.srpfmp", std::ofstream::binary | std::ofstream::trunc);

	file.write(reinterpret_cast<const char*>(&mWidth), sizeof(int));
	file.write(reinterpret_cast<const char*>(&mHeight), sizeof(int));
	file.write(reinterpret_cast<const char*>(&mData[0]), sizeof(int)*mData.size());

	file.close();
}

void Map::load(std::string file) {
	std::ifstream ffile;
	ffile.open(file, std::ifstream::binary);

	ffile.read(reinterpret_cast<char*>(&mWidth), sizeof(int));
	ffile.read(reinterpret_cast<char*>(&mHeight), sizeof(int));

	mData.resize(mWidth*mHeight);
	ffile.read(reinterpret_cast<char*>(&mData[0]), sizeof(int)*mData.size());

	ffile.close();
}

void Map::init() {
	mData.resize(mWidth*mHeight);
	for (unsigned int i=0; i<mData.size(); ++i)
		mData[i] = 0;
}
