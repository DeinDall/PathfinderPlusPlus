#ifndef MOUSEDRAWER_H
#define MOUSEDRAWER_H

#include <SFML/Graphics.hpp>
#include "map.h"

std::vector<sf::Vector2i> genLineBetween(sf::Vector2i start, sf::Vector2i end);

class AbstractCursorState {
public:
	virtual ~AbstractCursorState();

	virtual void onMousePress(sf::Mouse::Button button, sf::Vector2i mpos) = 0;
	virtual void onMouseMove(sf::Vector2i mpos) = 0;
	virtual void onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos) = 0;

	/// Returns whenever the cursor should be current, not whenever it is doing something.
	virtual bool isActive() const = 0;
};

class SetPosCursor : public AbstractCursorState {
public:
	SetPosCursor(sf::Vector2i& target);

	void onMousePress(sf::Mouse::Button button, sf::Vector2i mpos);
	void onMouseMove(sf::Vector2i mpos);
	void onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos);

	bool isActive() const;

private:
	sf::Vector2i& mTarget;
	bool mDone;
};

class DrawFreeCursor : public AbstractCursorState {
public:
	DrawFreeCursor(Map& target);

	void onMousePress(sf::Mouse::Button button, sf::Vector2i mpos);
	void onMouseMove(sf::Vector2i mpos);
	void onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos);

	bool isActive() const;

private:
	Map& mTarget;
	sf::Vector2i mLastPos;
	bool mDrawing, mErase;
};

class DrawLineCursor : public AbstractCursorState {
public:
	DrawLineCursor(Map& target);

	void onMousePress(sf::Mouse::Button button, sf::Vector2i mpos);
	void onMouseMove(sf::Vector2i mpos);
	void onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos);

	bool isActive() const;

private:
	Map& mTarget;
	sf::Vector2i mStartPos;
	bool mDrawing, mErase;
};

class DrawCursor {
public:
	DrawCursor(sf::IntRect drawArea);
	~DrawCursor();

	void setColor(const sf::Color& color);

	bool hasState() const;

	void onMousePress(sf::Mouse::Button button, sf::Vector2i mpos);
	void onMouseMove(sf::Vector2i mpos);
	void onMouseRelease(sf::Mouse::Button button, sf::Vector2i mpos);

	void draw(sf::RenderWindow& window);

	template<class CurT, typename... Args>
	void setCursorState(Args&& ... args) {
		setStatePtr(new CurT(std::forward<Args>(args)...));
	}

protected:
	void setStatePtr(AbstractCursorState* cursor);

private:
	AbstractCursorState* mState;

	sf::IntRect mDrawArea;
	sf::Color mColor;

	sf::Vector2i mLastPos;
};

#endif // MOUSEDRAWER_H
