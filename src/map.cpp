#include "map.h"

#include <fstream>

Map::Map(unsigned int w, unsigned int h) {
	mWidth = w;
	mHeight = h;

	init();
}

Map::~Map() {}

bool Map::get(unsigned int x, unsigned int y) const {
	return mData[y*mWidth+x];
}

void Map::set(unsigned int x, unsigned int y, bool val) {
	mData[y*mWidth+x] = val;
}

int Map::width() const {
	return mWidth;
}

int Map::height() const {
	return mHeight;
}

void Map::clear() {
	for (unsigned int i=0; i<mData.size(); ++i)
		mData[i] = false;
}

void Map::save(std::string file) {
	std::ofstream ffile;
	ffile.open(file, std::ofstream::binary | std::ofstream::trunc);

	ffile.write(reinterpret_cast<const char*>(&mWidth), sizeof(int));
	ffile.write(reinterpret_cast<const char*>(&mHeight), sizeof(int));
//	ffile.write(reinterpret_cast<const char*>(&mData.data()), );

	ffile.close();
}

void Map::load(std::string file) {
	std::ifstream ffile;
	ffile.open(file, std::ifstream::binary);

	ffile.read(reinterpret_cast<char*>(&mWidth), sizeof(int));
	ffile.read(reinterpret_cast<char*>(&mHeight), sizeof(int));

//	mData.resize(mWidth*mHeight);
//	ffile.read(reinterpret_cast<char*>(&mData[0]), sizeof(tile)*mData.size());

	ffile.close();
}

void Map::init() {
	mData.resize(mWidth*mHeight);
	for (unsigned int i=0; i<mData.size(); ++i)
		mData[i] = false;
}
