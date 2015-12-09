#ifndef PERSO_H
#define PERSO_H

// c'est sale, mais ça marche
#define PERSO_IMAGE_SPEED 0.4
#define PERSO_IMAGE_COUNT 6

#define PERSO_SUBIMAGE_HEIGHT 80
#define PERSO_SUBIMAGE_WIDTH 32

#include <SFML/Graphics.hpp>
#include "direction.h"

class Perso {
public:
	Perso(int x, int y) {
		mX = x;
		mY = y;

		mSprite.setTexture(mTexture);
		setImageIndex(0, 0);
	}

	void update() {
		// animation de l'image du personnage
		mImageIndex += PERSO_IMAGE_SPEED;

		if (mImageIndex > PERSO_IMAGE_COUNT)
			mImageIndex -= PERSO_IMAGE_COUNT;

		// animation de la position du personnage
		if (isMoving()) {
			setImageIndex(mImageDirection, (int) mImageIndex);
			mMovingAnimTimer--;
		} else
			setImageIndex(mImageDirection, 0);

		// positionnement du sprite
		mSprite.setPosition(mX*32 - mDirection.xOffset*mMovingAnimTimer,
							mY*32 - mDirection.yOffset*mMovingAnimTimer - (PERSO_SUBIMAGE_HEIGHT-32));
	}

	void draw(sf::RenderWindow& window) {
		window.draw(mSprite);
	}

	bool isMoving() {
		return mMovingAnimTimer>0;
	}

protected:
	//
	void setImageIndex(int direction, int index) {
		mSprite.setTextureRect(sf::IntRect(direction*PERSO_SUBIMAGE_WIDTH,
										   index*PERSO_SUBIMAGE_HEIGHT,
										   PERSO_SUBIMAGE_WIDTH,
										   PERSO_SUBIMAGE_HEIGHT));
	}

private:
	// x et y: les coordonnées sont selon la map, et non selon l'écran.
	int mX, mY;

	int mImageDirection;
	float mImageIndex;

	int mMovingAnimTimer;
	Direction mDirection;

	sf::Texture mTexture;
	sf::Sprite mSprite;
};

#endif // PERSO_H
