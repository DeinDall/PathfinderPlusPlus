#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button {
public:
	Button(sf::IntRect geometry, int buttonId);

	void onMousePressed(sf::Vector2i mousePos);

	void onMouseReleased(sf::Vector2i mousePos);

	void setContents(std::wstring text);

	bool getClicked();

	int getId();

	void updateGraphics();

	void update(sf::Vector2i mousePos);

	void draw(sf::RenderWindow& window);

	static sf::Font& getFont();

protected:
	sf::Vector2f getContentsPosition();

	void setImage(int image);

private:
	sf::IntRect mGeometry;

	bool mIsClicking;
	bool mClicked;

	int mButtonId;

	int mImage;

	sf::Text mContents;
	sf::VertexArray mGraphics;
	bool mGraphicsNeedsUpdate;

	static bool msGraphicsInitialized;
	static sf::Texture msTexture;
	static sf::Font msFont;
};

#endif // BUTTON_H
