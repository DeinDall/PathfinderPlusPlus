#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <fstream>

class Map {
public:
	// Constructeur(w, h): Construit la carte avec la taille donnée.
	Map(int w, int h);

	// get(x, y): Cette methode retourne la valeure de la tuile aux coordonées données (/!\: non sécurisé).
	int get(int x, int y);

	// set(x, y, val): Cette methode modifie la valeure de la tuile aux coordonées données (/!\: non sécurisé).
	void set(int x, int y, int val);

	// width(): Cette methode retourne la largeur de la carte
	int width();

	// height(): Cette methode retourne la hauteur de la carte
	int height();

	void save();

	void load(std::string file);

protected:
	// init(): Cette methode initialise le tableau de données selon la taille de la carte.
	void init();

private:
	std::vector<int> mData;
	int mWidth, mHeight;
};

#endif // MAP_H
