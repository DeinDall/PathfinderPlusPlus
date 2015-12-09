#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Map {
public:
	// Constructeur(w, h): Construit la carte avec la taille donnée.
	Map(unsigned int w, unsigned int h);
	~Map();

	// get(x, y): Cette methode retourne la valeure de la tuile aux coordonées données (/!\: non sécurisé).
	bool get(unsigned int x, unsigned int y) const;

	// set(x, y, val): Cette methode modifie la valeure de la tuile aux coordonées données (/!\: non sécurisé).
	void set(unsigned int x, unsigned int y, bool val);

	// width(): Cette methode retourne la largeur de la carte
	int width() const;

	// height(): Cette methode retourne la hauteur de la carte
	int height() const;

	void clear();

	void save(std::string file);

	void load(std::string file);

protected:
	// init(): Cette methode initialise le tableau de données selon la taille de la carte.
	void init();

private:
	std::vector<bool> mData;
	unsigned int mWidth, mHeight;
};

#endif // MAP_H
