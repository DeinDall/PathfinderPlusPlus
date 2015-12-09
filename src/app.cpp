#include "app.h"
#include "util/dialogs.h"

App::App()
	: mFontPath("./resources/font.ttf"),
	  mMap(24, 18),
	  mMouseDrawer(sf::IntRect(0, 0, 768, 576)),
	  mCurrentCursor(MOUSEDRAW),
	  mSfgWindow(sfg::Window::Create(sfg::Window::BACKGROUND)),
	  mConsole(mSfgDesktop.GetEngine().GetResourceManager().GetFont(mFontPath)) {
	initGraphics();
	initGUI();
}

App::~App() {}

#include "graphics/components/animatedsprite.h"

int App::run() {
	sf::Event ev;
	sf::Clock loopClock;

	sf::IntRect mapViewRect(0, 0, 768, 576);

	mStartPos = sf::Vector2i(6, 4);
	mEndPos = sf::Vector2i(mMap.width()-7, mMap.height()-5);

	computePath();

	while (mWindow.isOpen()) {
		// Ici, on commence à parcourir les évènements envoyés par la fenêtre
		while (mWindow.pollEvent(ev)) {
			if (ev.type==sf::Event::Closed)
				mWindow.close();
			else {
				mSfgDesktop.HandleEvent(ev);

				if (ev.type==sf::Event::MouseButtonPressed) {
					sf::Vector2i mpos(ev.mouseButton.x, ev.mouseButton.y);
					if ((mCurrentCursor == MOUSEDRAW) && mapViewRect.contains(mpos)) {
						if (ev.mouseButton.button==sf::Mouse::Right)
							mMouseDrawer.onMousePress(mpos, true);
						else if (ev.mouseButton.button==sf::Mouse::Left)
							mMouseDrawer.onMousePress(mpos, false);
					}
				} else if (ev.type==sf::Event::MouseButtonReleased) {
					sf::Vector2i mpos(ev.mouseButton.x, ev.mouseButton.y);

					if (mapViewRect.contains(mpos)) {
						switch (mCurrentCursor) {
						case MOUSEDRAW:
							if (ev.mouseButton.button==sf::Mouse::Right)
								mMouseDrawer.onMouseRelease(mpos, true);
							else if (ev.mouseButton.button==sf::Mouse::Left)
								mMouseDrawer.onMouseRelease(mpos, false);
							break;
						case PLACE_START:
							mStartPos = mpos/32;
							computePath();
							mCurrentCursor = MOUSEDRAW;
							break;
						case PLACE_END:
							mEndPos = mpos/32;
							computePath();
							mCurrentCursor = MOUSEDRAW;
							break;
						default:
							break;
						}
					}
				}
			}
		}

		mSfgDesktop.Update(loopClock.restart().asSeconds());

		// Ici on met à jour les éléments interractifs
		mMouseDrawer.update(sf::Mouse::getPosition(mWindow), mMap);

		// Ici, on met les cases du début et de la fin comme vierges, afin nottament d'éviter les anomalies graphiques.
		mMap.set(mStartPos.x, mStartPos.y, false);
		mMap.set(mEndPos.x, mEndPos.y, false);

		// Ici, on change la couleur du cruseur sur la map, en fonction de ce que l'on place (début, fin, ou "bloc")
		switch (mCurrentCursor) {
		case PLACE_START:
			mMouseDrawer.setColor(sf::Color(255, 128, 64, 128));
			break;
		case PLACE_END:
			mMouseDrawer.setColor(sf::Color(64, 255, 128, 128));
			break;
		default:
			mMouseDrawer.setColor(sf::Color(64, 128, 255, 128));
			break;
		}

		mConsole.update();

		if (mPathFinder.shouldContinue()) {
			mPathFinder.forwardPathfind();
			mPathFinder.generatePath();
			mPathGraphics.updatePath(mPathFinder.getStartPos(), mPathFinder.getPath());
		}

		mMapGraphics.update(mMap);

		// Le dessin commence ici.
		mWindow.clear();

		{ // C'est ici que l'on dessine les éléments par lignes
			sf::RectangleShape rect(sf::Vector2f(28, 28));

			for (int i=0; i<mMap.height(); ++i) {
				mMapGraphics.drawWallLayer(mWindow, i);

				if (mStartPos.y == i) {
					rect.setPosition(mStartPos.x*32+2, mStartPos.y*32+2);
					rect.setFillColor(sf::Color(255, 128, 64, 255));
					mWindow.draw(rect);
				}
				if (mEndPos.y == i) {
					rect.setPosition(mEndPos.x*32+2, mEndPos.y*32+2);
					rect.setFillColor(sf::Color(64, 255, 128, 255));
					mWindow.draw(rect);
				}
			}
		}

		mPathGraphics.draw(mWindow, mPathFinder.getContext());
		mMouseDrawer.draw(mWindow);

		mConsole.draw(mWindow);
		mSFGUI.Display(mWindow);

		mWindow.display();
	}

	return 0;
}

void App::initGraphics() {
	mWindow.create(sf::VideoMode(1024, 576), "Pathfinder++", sf::Style::Close);

	mWindow.setFramerateLimit(60);
	mWindow.resetGLStates();
}

void App::initGUI() {
	mSfgDesktop.SetProperty("*", "FontName", mFontPath);

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);

	auto btn = sfg::Button::Create(L"Rafraichir le chemin");
	btn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&App::computePath, this));
	box->Pack(btn, false);

	auto tbtn = sfg::CheckButton::Create(L"Afficher les noeuds");
	tbtn->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&App::buttonToggleNodes, this, tbtn));
	box->Pack(tbtn, false);

	box->Pack(sfg::Separator::Create(), false);

	auto frame = sfg::Frame::Create("Dessin");

	{
		auto hbox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);

		auto radio = sfg::RadioButton::Create(L"Libre");
		radio->SetActive(true);
		radio->GetSignal(sfg::RadioButton::OnToggle).Connect(std::bind(&App::changeDrawerMethod, this, MouseDrawer::DRAW_FREE));
		hbox->Pack(radio);

		radio = sfg::RadioButton::Create(L"Lignes", radio->GetGroup());
		radio->GetSignal(sfg::RadioButton::OnToggle).Connect(std::bind(&App::changeDrawerMethod, this, MouseDrawer::DRAW_LINES));
		hbox->Pack(radio);

		frame->Add(hbox);
	}

	box->Pack(frame, false);
	box->Pack(sfg::Separator::Create(), false);

	btn = sfg::Button::Create(L"Placer le début");
	btn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&App::changeCursor, this, PLACE_START));
	box->Pack(btn, false);

	btn = sfg::Button::Create(L"Placer la fin");
	btn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&App::changeCursor, this, PLACE_END));
	box->Pack(btn, false);

	box->Pack(sfg::Separator::Create(), false);

	btn = sfg::Button::Create(L"Remettre à zéro");
	btn->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&App::clearMap, this));
	box->Pack(btn, false);

	mSfgWindow->Add(box);
	mSfgWindow->SetAllocation(sf::FloatRect(768, 0, 256, 576));

	mSfgDesktop.Add(mSfgWindow);
}

void App::computePath() {
	mPathFinder.startPathfind(new PathContext(mMap, mStartPos.x, mStartPos.y, mEndPos.x, mEndPos.y));
}

void App::changeCursor(CursorType cursor) {
	mCurrentCursor = cursor;
}

void App::changeDrawerMethod(MouseDrawer::DrawMethod method) {
	mMouseDrawer.setMethod(method);
}

void App::buttonToggleNodes(sfg::CheckButton::Ptr btn) {
	mPathGraphics.setShowingNodes(btn->IsActive());
}

void App::clearMap() {
	if (dialogs::openMessage("Attention !", "Voulez-vous vraiment supprimer toute la carte ?",
							 dialogs::LevelWarning, dialogs::ButtonsYesNo))
		mMap.clear();
}
