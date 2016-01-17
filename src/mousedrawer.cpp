#include "mousedrawer.h"

AbstractCursorState::~AbstractCursorState() {}

SetPosCursor::SetPosCursor(sf::Vector2i& target)
	: mTarget(target), mDone(false) {}

void SetPosCursor::onMousePress(sf::Mouse::Button button, sf::Vector2i mpos) {
	mTarget = mpos/32;
	mDone = true;
}

void SetPosCursor::onMouseMove(sf::Vector2i mpos) {}
void SetPosCursor::onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos) {}

bool SetPosCursor::isActive() const {
	return !mDone;
}

// -------

DrawFreeCursor::DrawFreeCursor(Map& target)
	: mTarget(target), mLastPos(0, 0), mDrawing(false), mErase(false) {}

void DrawFreeCursor::onMousePress(sf::Mouse::Button button, sf::Vector2i mpos) {
	mErase = (button == sf::Mouse::Right);
	mLastPos = mpos/32;
	mDrawing = true;

	mTarget.set(mLastPos.x, mLastPos.y, !mErase);
}

void DrawFreeCursor::onMouseMove(sf::Vector2i mpos) {
	if (mDrawing) {
		sf::Vector2i npos = mpos/32;

		for (sf::Vector2i pos : genLineBetween(mLastPos, npos))
			mTarget.set(pos.x, pos.y, !mErase);

		mLastPos = npos;
	}
}

void DrawFreeCursor::onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos) {
	mDrawing = false;
}

bool DrawFreeCursor::isActive() const {
	return true;
}

// -------

DrawLineCursor::DrawLineCursor(Map& target)
	: mTarget(target), mStartPos(0, 0), mDrawing(false), mErase(false) {}

void DrawLineCursor::onMousePress(sf::Mouse::Button button, sf::Vector2i mpos) {
	mErase = (button == sf::Mouse::Right);
	mStartPos = mpos/32;
	mDrawing = true;
}

void DrawLineCursor::onMouseMove(sf::Vector2i mpos) {}

void DrawLineCursor::onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos) {
	if (mDrawing) {
		for (sf::Vector2i pos : genLineBetween(mStartPos, mpos/32))
			mTarget.set(pos.x, pos.y, !mErase);
	}
}

bool DrawLineCursor::isActive() const {
	return true;
}

// -------

DrawCursor::DrawCursor(sf::IntRect drawArea)
	: mDrawArea(drawArea) {
	mState = nullptr;
}

DrawCursor::~DrawCursor() {
	if (mState)
		delete mState;
}

void DrawCursor::setColor(const sf::Color& color) {
	mColor = color;
}

void DrawCursor::setStatePtr(AbstractCursorState* cursor) {
	if (mState)
		delete mState;
	mState = cursor;
}

bool DrawCursor::hasState() const {
	return (mState!=nullptr);
}

void DrawCursor::onMousePress(sf::Mouse::Button button, sf::Vector2i mpos) {
	if (mState && mDrawArea.contains(mpos)) {
		mState->onMousePress(button, mpos);
		if (!mState->isActive())
			setStatePtr(nullptr);
	}
}

void DrawCursor::onMouseMove(sf::Vector2i mpos) {
	if (mState && mDrawArea.contains(mpos)) {
		mState->onMouseMove(mpos);
		if (!mState->isActive())
			setStatePtr(nullptr);

		mLastPos = mpos/32;
	}
}

void DrawCursor::onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos) {
	if (mState && mDrawArea.contains(mpos)) {
		mState->onMouseRelease(button, mpos);
		if (!mState->isActive())
			setStatePtr(nullptr);
	}
}

void DrawCursor::draw(sf::RenderWindow &window) {
	sf::RectangleShape shape(sf::Vector2f(32, 32));
	shape.setPosition(mLastPos.x*32, mLastPos.y*32);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(1);
	shape.setOutlineColor(mColor);
	window.draw(shape);
}

// -------

std::vector<sf::Vector2i> genLineBetween(sf::Vector2i start, sf::Vector2i end) {
	std::vector<sf::Vector2i> poslist;
	sf::Vector2i diff = (end-start);
	unsigned int size = std::max(std::abs(diff.x), std::abs(diff.y));

	poslist.resize(size+1);
	poslist[size] = end;

	for (unsigned int i=0; i<size; ++i) {
		float a=((float)(i+0.5)/size);
		poslist[i] = start+sf::Vector2i(diff.x*a, diff.y*a);
	}

	return std::move(poslist);
}
