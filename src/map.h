#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Map {
public:
	Map(unsigned int w, unsigned int h);
	~Map();

	bool get(unsigned int x, unsigned int y) const;
	void set(unsigned int x, unsigned int y, bool val);

	int width() const;
	int height() const;

	void clear();

	void save(std::string file);
	void load(std::string file);

protected:
	void init();

private:
	std::vector<bool> mData;
	unsigned int mWidth, mHeight;
};

#endif // MAP_H
