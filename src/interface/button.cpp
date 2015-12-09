#include "button.h"

bool Button::msGraphicsInitialized(false);
sf::Texture Button::msTexture;
sf::Font Button::msFont;

Button::Button(sf::IntRect geometry, int buttonId) : mGeometry(geometry) {
	mGraphicsNeedsUpdate = true;
	mGraphics.setPrimitiveType(sf::Quads);

	if (!msGraphicsInitialized) {
		msTexture.loadFromFile("resources/interface.png");
		msFont.loadFromFile("resources/font.ttf");
	}

	mContents.setCharacterSize(geometry.height-16);
	mContents.setFont(msFont);
	mContents.setColor(sf::Color::Black);

	mIsClicking = false;
	mClicked = false;
	mButtonId = buttonId;

	mImage = -1;
}

void Button::onMousePressed(sf::Vector2i mousePos) {
	if (mGeometry.contains(mousePos))
		mIsClicking = true;
}

void Button::onMouseReleased(sf::Vector2i mousePos) {
	if (mGeometry.contains(mousePos)) {
		mIsClicking = false;
		mClicked = true;
	}
}

void Button::setContents(std::wstring text) {
	mContents.setString(sf::String(text));
}

bool Button::getClicked() {
	if (mClicked) {
		mClicked = false;
		return true;
	}

	return false;
}

int Button::getId() {
	return mButtonId;
}

void Button::updateGraphics() {
	mGraphics.resize(36); // 9*4

	// the image offset
	int o=mImage*24;

	int x1 = mGeometry.left,
		x2 = x1 + 8,
		x4 = mGeometry.left + mGeometry.width,
		x3 = x4 - 8;

	int y1 = mGeometry.top,
		y2 = y1 + 8,
		y4 = mGeometry.top + mGeometry.height,
		y3 = y4 - 8;

	// y1
	mGraphics[0] = sf::Vertex(sf::Vector2f(x1, y1), sf::Vector2f(o+0, 0));
	mGraphics[1] = mGraphics[4] = sf::Vertex(sf::Vector2f(x2, y1), sf::Vector2f(o+8, 0));
	mGraphics[5] = mGraphics[8] = sf::Vertex(sf::Vector2f(x3, y1), sf::Vector2f(o+16, 0));
	mGraphics[9] = sf::Vertex(sf::Vector2f(x4, y1), sf::Vector2f(o+24, 0));

	// y2
	mGraphics[3] = mGraphics[12] = sf::Vertex(sf::Vector2f(x1, y2), sf::Vector2f(o+0, 8));
	mGraphics[2] = mGraphics[7] = mGraphics[13] = mGraphics[16] = sf::Vertex(sf::Vector2f(x2, y2), sf::Vector2f(o+8, 8));
	mGraphics[6] = mGraphics[11] = mGraphics[17] = mGraphics[20] = sf::Vertex(sf::Vector2f(x3, y2), sf::Vector2f(o+16, 8));
	mGraphics[10] = mGraphics[21] = sf::Vertex(sf::Vector2f(x4, y2), sf::Vector2f(o+24, 8));

	// y3
	mGraphics[15] = mGraphics[24] = sf::Vertex(sf::Vector2f(x1, y3), sf::Vector2f(o+0, 16));
	mGraphics[14] = mGraphics[19] = mGraphics[28] = mGraphics[25] = sf::Vertex(sf::Vector2f(x2, y3), sf::Vector2f(o+8, 16));
	mGraphics[18] = mGraphics[23] = mGraphics[32] = mGraphics[29] = sf::Vertex(sf::Vector2f(x3, y3), sf::Vector2f(o+16, 16));
	mGraphics[22] = mGraphics[33] = sf::Vertex(sf::Vector2f(x4, y3), sf::Vector2f(o+24, 16));

	// y4
	mGraphics[27] = sf::Vertex(sf::Vector2f(x1, y4), sf::Vector2f(o+0, 24));
	mGraphics[26] = mGraphics[31] = sf::Vertex(sf::Vector2f(x2, y4), sf::Vector2f(o+8, 24));
	mGraphics[30] = mGraphics[35] = sf::Vertex(sf::Vector2f(x3, y4), sf::Vector2f(o+16, 24));
	mGraphics[34] = sf::Vertex(sf::Vector2f(x4, y4), sf::Vector2f(o+24, 24));
}

void Button::update(sf::Vector2i mousePos) {
	if (mGeometry.contains(mousePos)) {
		if (mIsClicking)
			setImage(2);
		else
			setImage(1);
	} else {
		if (mIsClicking)
			mIsClicking = false;
		setImage(0);
	}
}

void Button::draw(sf::RenderWindow& window) {
	if (mGraphicsNeedsUpdate)
		updateGraphics();

	mContents.setPosition(getContentsPosition());

	sf::RenderStates states;
	states.texture = &msTexture;

	window.draw(mGraphics, states);
	window.draw(mContents);
}

sf::Font& Button::getFont() {
	return msFont;
}

sf::Vector2f Button::getContentsPosition() {
	return sf::Vector2f(mGeometry.left+mIsClicking+8, mGeometry.top+mIsClicking+8);
}

void Button::setImage(int image) {
	mGraphicsNeedsUpdate = (image != mImage);
	mImage = image;
}
