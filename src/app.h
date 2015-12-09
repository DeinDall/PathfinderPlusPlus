#ifndef APP_H
#define APP_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "graphics/console.h"

#include "map.h"
#include "graphics/mapgraphics.h"

#include "path/pathfinder.h"
#include "graphics/pathgraphics.h"

#include "mousedrawer.h"

class App {
	enum CursorType {
		MOUSEDRAW=0,
		PLACE_START=1,
		PLACE_END=2
	};
public:
	App();
	~App();

	int run();

private:
	void initGraphics();
	void initGUI();

private:
	void computePath();
	void changeCursor(CursorType cursor);
	void changeDrawerMethod(MouseDrawer::DrawMethod method);
	void buttonToggleNodes(sfg::CheckButton::Ptr btn);
	void clearMap();

private:
	std::string mFontPath;
	// Logic components
	Map mMap;
	MapGraphics mMapGraphics;

	sf::Vector2i mStartPos, mEndPos;

	PathFinder mPathFinder;
	PathGraphics mPathGraphics;

	MouseDrawer mMouseDrawer;
	CursorType mCurrentCursor;

	// SFML components
	sf::RenderWindow mWindow;

	// SFGUI components
	sfg::SFGUI mSFGUI;
	sfg::Desktop mSfgDesktop;

	sfg::Window::Ptr mSfgWindow;

	// Other GUI components
	Console mConsole;
};

#endif // APP_H
