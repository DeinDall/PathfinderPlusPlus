#include "animatedsprite.h"

AnimatedSprite::AnimatedSprite()
	: mTexture(nullptr), mFrameSize(0, 0),
	  mMultiLineAnim(true), mAnimSpeed(1.f),
	  mCanDraw(false), mDrawNeedsUpdate(true), mStatsNeedsUpdate(true),
	  mImageCount(0), mRowCount(0), mImage(0)  {
	mVertexArray.setPrimitiveType(sf::TrianglesStrip);
	mVertexArray.resize(4);
}

AnimatedSprite::AnimatedSprite(const sf::Texture& texture, float speed)
	: AnimatedSprite() {
	setTexture(texture);
	setAnimationSpeed(speed);
}

AnimatedSprite::AnimatedSprite(const sf::Texture& texture, sf::Vector2f frameSize, float speed)
	: AnimatedSprite(texture, speed) {
	setFrameSize(frameSize);
}

AnimatedSprite::~AnimatedSprite() {}

void AnimatedSprite::setTexture(const sf::Texture& texture) {
	const sf::Texture* prevTexture = std::move(mTexture);
	mTexture = &texture;

	if (mTexture != prevTexture)
		mStatsNeedsUpdate = true;
}

const sf::Texture& AnimatedSprite::getTexture() const {
	return *mTexture;
}

void AnimatedSprite::setFrameSize(sf::Vector2f fsize) {
	if (fsize==sf::Vector2f(0, 0)) {
		if (mTexture != nullptr) {
			sf::Vector2u texSize = mTexture->getSize();

			if (texSize!=sf::Vector2u(0, 0))
				setFrameSize(sf::Vector2f(texSize.x, texSize.y));
		}
	} else {
		sf::Vector2f prevSize = std::move(mFrameSize);
		mFrameSize = fsize;

		if (prevSize != mFrameSize)
			mStatsNeedsUpdate = true;
	}
}

sf::Vector2f AnimatedSprite::getFrameSize() const {
	return mFrameSize;
}

void AnimatedSprite::setFrameIndex(unsigned int frame) {
	unsigned int curImage = (unsigned int)mImage;
	mImage = frame;

	if (frame != curImage)
		mDrawNeedsUpdate = true;
}

unsigned int AnimatedSprite::getFrameIndex() const {
	return (unsigned int)mImage;
}

void AnimatedSprite::setAnimationSpeed(float speed) {
	mAnimSpeed = speed;
}

float AnimatedSprite::getAnimationSpeed() const {
	return mAnimSpeed;
}

void AnimatedSprite::setAnimateMultipleLines(bool doAnim) {
	bool prevValue = mMultiLineAnim;
	mMultiLineAnim = doAnim;

	if (prevValue != mMultiLineAnim)
		mStatsNeedsUpdate = true;
}

bool AnimatedSprite::getAnimateMultipleLines() const {
	return mMultiLineAnim;
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	pUpdateGraphics();

	if (mCanDraw) {
		states.texture = mTexture;
		states.transform.combine(getTransform());
		target.draw(mVertexArray, states);
	}
}

sf::Vector2f operator*(sf::Vector2f f, sf::Vector2f s) {
	return sf::Vector2f(f.x*s.x, f.y*s.y);
}

void AnimatedSprite::pUpdateGraphics() const {
	if (mStatsNeedsUpdate)
		pUpdateAnimStats();

	if (mImageCount*mRowCount==0 || mTexture==nullptr) {
		mCanDraw = false;
		return;
	}

	unsigned int realImage = (unsigned int)mImage;
	mImage += mAnimSpeed;

	if (mImage >= mImageCount*mRowCount)
		mImage -= mImageCount*mRowCount;

	if (realImage != (unsigned int)mImage)
		mDrawNeedsUpdate = true;

	if (mDrawNeedsUpdate) {
		realImage = (unsigned int)mImage;

		sf::Vector2f tex(realImage%mImageCount, realImage/mImageCount);
		tex = tex*mFrameSize;

		static const sf::Vector2f offset[4] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(0, 1),
			sf::Vector2f(1, 0),
			sf::Vector2f(1, 1)
		};

		mVertexArray[0] = sf::Vertex(offset[0]*mFrameSize, tex + offset[0]*mFrameSize);
		mVertexArray[1] = sf::Vertex(offset[1]*mFrameSize, tex + offset[1]*mFrameSize);
		mVertexArray[2] = sf::Vertex(offset[2]*mFrameSize, tex + offset[2]*mFrameSize);
		mVertexArray[3] = sf::Vertex(offset[3]*mFrameSize, tex + offset[3]*mFrameSize);

		mDrawNeedsUpdate = false;
		mCanDraw = true;
	}
}

void AnimatedSprite::pUpdateAnimStats() const {
	sf::Vector2u texSize = mTexture->getSize();

	unsigned int lastRowCount = mRowCount;
	unsigned int lastImgCount = mImageCount;

	if (mMultiLineAnim)
		mRowCount = texSize.y / mFrameSize.y;
	else
		mRowCount = (mFrameSize.y>=texSize.y ? 1 : 0);

	mImageCount = texSize.x / mFrameSize.x;

	if (lastRowCount != mRowCount || lastImgCount != mImageCount)
		mDrawNeedsUpdate = true;

	mStatsNeedsUpdate = false;
}
