#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Drawable, public sf::Transformable {
public:
	AnimatedSprite();
	AnimatedSprite(const sf::Texture& texture, float speed = 1.f);
	AnimatedSprite(const sf::Texture& texture, sf::Vector2f frameSize, float speed = 1.f);

	AnimatedSprite(const AnimatedSprite&) = delete; // copy
	AnimatedSprite(AnimatedSprite&&) = delete; // move

	~AnimatedSprite();

	void setTexture(const sf::Texture& texture);
	const sf::Texture& getTexture() const;

	void setFrameSize(sf::Vector2f fsize);
	sf::Vector2f getFrameSize() const;

	void setFrameIndex(unsigned int frame);
	unsigned int getFrameIndex() const;

	void setAnimationSpeed(float speed);
	float getAnimationSpeed() const;

	void setAnimateMultipleLines(bool doAnim);
	bool getAnimateMultipleLines() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void pUpdateGraphics() const;
	void pUpdateAnimStats() const;

private:
	const sf::Texture* mTexture;
	sf::Vector2f mFrameSize;
	bool mMultiLineAnim;
	float mAnimSpeed;

	// Internal
	mutable bool mCanDraw, mDrawNeedsUpdate, mStatsNeedsUpdate;
	mutable unsigned int mImageCount, mRowCount;
	mutable float mImage;
	mutable sf::VertexArray mVertexArray;
};

#endif // ANIMATEDSPRITE_H
