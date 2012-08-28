#include "Game.h"
#include "utils.h"

// Ctor
Game::Game(sf::RenderWindow & window)
	// Fenetrage       // Evènements
	: mWindow(window), mActionMap(mWindow)
{
	// Etats du jeu
	mPaused = false;
	mExited = false;

	mCurrentZoom = 1.f;
}

// Dtor
Game::~Game(void)
{
}

// Boucle de jeu
void Game::Run()
{
	// Appel l'initialisation
	this->OnInit();

	// Tant que la fenêtre est ouverte
	while (mWindow.isOpen() && !mExited)
	{
		// La boucle commence
		this->OnLoopBegin();

		// Appel des évènements
		this->OnEvent();

		// Rendu
		this->OnRender();

		// La boucle se termine
		this->OnLoopEnd();
	}

	// On quite
	this->OnQuit();
}

/* Evènements du jeu */
/// Initialise le jeu
void Game::OnInit()
{
	/* Fenêtrage */
	// Crée la vue
	mView = mWindow.getDefaultView();
	mView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	/* Crée les actions */
	// Action de fermeture de la fenêtre + callback
	mActionMap["closed"] = thor::Action(sf::Keyboard::Escape, thor::Action::ReleaseOnce) || thor::Action(sf::Event::Closed);
	//mActionCallbackSystem.connect("closed", std::bind(&sf::RenderWindow::close, &mWindow));

	// Action de zoom + resize
	mActionMap["zoomIn"] = thor::Action(sf::Keyboard::Add, thor::Action::PressOnce);
	mActionMap["zoomOut"] = thor::Action(sf::Keyboard::Subtract, thor::Action::PressOnce);
	mActionMap["zoomReset"] = thor::Action(sf::Keyboard::Numpad0, thor::Action::PressOnce);
	mActionMap["resized"] = thor::Action(sf::Event::Resized);
	mActionMap["mouseWheelMoved"] = thor::Action(sf::Event::MouseWheelMoved);
	mActionCallbackSystem.connect("mouseWheelMoved", OnMouseWheelMoved(this));

	// Actions pour la souris
	mActionMap["movingView"] = thor::Action(sf::Mouse::Middle, thor::Action::Hold);
}

/// Appelé quand la boucle commence
void Game::OnLoopBegin()
{
	// Sauvegarde la dernière position de la souris
	mCurrentMousePos = i2f(sf::Mouse::getPosition());
	mCurrentMousePosRV = mWindow.convertCoords(sf::Mouse::getPosition(mWindow), mView);
}

/// Appelé pour les évènements
void Game::OnEvent()
{
	/* Evenements */
	// Récupère les évènements
	mActionMap.update();

	// Invoque les callbacks
	mActionMap.invokeCallbacks(mActionCallbackSystem);

	// Gestion de la fermeture de la fenêtre
	if (mActionMap.isActive("closed"))
	{
		mExited = true;
		mWindow.close();
	}

	// Gestion du zoom et du resize
    if (mActionMap.isActive("zoomIn"))
	{
		mCurrentZoom *= 0.8f;
		mView.zoom(0.8f);
	}
    if (mActionMap.isActive("zoomOut"))
	{
		mCurrentZoom *= 1.2f;
		mView.zoom(1.2f);
	}
    if (mActionMap.isActive("zoomReset"))
	{
		mCurrentZoom = 1.f;
		mView.setSize(mWindow.getSize().x * mView.getViewport().width, mWindow.getSize().y * mView.getViewport().height);
		mView.setCenter(sf::Vector2f(0.f, 0.f));
	}
    if (mActionMap.isActive("resized"))
	{
		mView.setCenter(sf::Vector2f(0.f, 0.f));
		mView.setSize(mWindow.getSize().x * mView.getViewport().width, mWindow.getSize().y * mView.getViewport().height);
		mView.zoom(mCurrentZoom);
	}

	// Gère le déplacement à la souris (clic molette)
	if (mActionMap.isActive("movingView"))
	{
		mView.move(- static_cast<float>(mCurrentMousePos.x - mLastMousePos.x) * mCurrentZoom,
						- static_cast<float>(mCurrentMousePos.y - mLastMousePos.y) * mCurrentZoom);
	}
}

/// Appelé pour le rendu
void Game::OnRender()
{
	// Affichage
	//mWindow.setView(mView);

	mWindow.clear(sf::Color::White);
	
	mWindow.display();
}

/// Appelé quand la boucle se termine
void Game::OnLoopEnd()
{
	// Sauvegarde la dernière position de la souris
	mLastMousePos = mCurrentMousePos;
	mLastMousePosRV = mCurrentMousePosRV;

	mElapsedTime.restart();
}

// Appelé quand le jeu se termine
void Game::OnQuit()
{
}

/* Pour la molette de la souris */
OnMouseWheelMoved::OnMouseWheelMoved(Game *_game)
: mGame(_game)
{
}
void OnMouseWheelMoved::operator() (thor::ActionContext<std::string> context)
{
	if (context.event->mouseWheel.delta > 0)
	{
		mGame->mCurrentZoom *= 0.8f;
		mGame->mView.zoom(0.8f);
		mGame->mWindow.setView(mGame->mView);
	}
	else
	{
		mGame->mCurrentZoom *= 1.2f;
		mGame->mView.zoom(1.2f);
		mGame->mWindow.setView(mGame->mView);
	}
}