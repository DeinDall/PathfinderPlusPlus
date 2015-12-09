#ifndef BUTTONS_H
#define BUTTONS_H

#include "button.h"
#include <list>
#include <queue>

class Buttons {
public:
	enum ButtonId {
		BTN_SHOWCASES,

		BTN_CLEAR,

		BTN_SETSTART,
		BTN_PATHSEARCH,
		BTN_RECOMPUTE,

		BTN_SAVE,
		BTN_LOAD,

		ButtonCount
	};

public:
	Buttons(sf::Vector2u offset, int width);
	~Buttons();

	Button& getButtonFromId(ButtonId id);

	void onMousePress(sf::Vector2i pos);
	void onMouseRelease(sf::Vector2i pos);

	void update(sf::Vector2i pos);
	void draw(sf::RenderWindow& window);

	bool popClick(ButtonId& id);

	int verticalOffset();

private:
	std::vector<Button*> mButtonList;
	std::queue<ButtonId> mClickedIds;
	int mVerticalOffset;
};

#endif // BUTTONS_H
