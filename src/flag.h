#ifndef FLAG_H
#define FLAG_H

#include <SFML/Graphics.hpp>

class Flag {
public:
	Flag(std::string flagTexName);

	void setPosition(sf::Vector2f pos);

	void update();
	void draw(sf::RenderWindow& window);

	void setImage(int img);

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;

	float mImage;
};

#endif // FLAG_H
