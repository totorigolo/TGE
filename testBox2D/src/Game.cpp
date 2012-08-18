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
	mWindow.setView(mView);

	/* Crée les actions */
	// Action de fermeture de la fenêtre + callback
	mActionMap["closed"] = thor::Action(sf::Keyboard::Escape, thor::Action::ReleaseOnce) || thor::Action(sf::Event::Closed);
	//mActionCallbackSystem.connect("closed", std::bind(&sf::RenderWindow::close, &mWindow));

	// Action de zoom + resize
	mActionMap["zoomIn"] = thor::Action(sf::Keyboard::Add, thor::Action::PressOnce);// || (thor::Action(sf::Event::MouseWheelMoved));
	mActionMap["zoomOut"] = thor::Action(sf::Keyboard::Subtract, thor::Action::PressOnce);// || (thor::Action(sf::Event::MouseWheelMoved));
	mActionMap["zoomReset"] = thor::Action(sf::Keyboard::Numpad0, thor::Action::PressOnce);
	mActionMap["resized"] = thor::Action(sf::Event::Resized);

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
		mWindow.setView(mView);
	}
    if (mActionMap.isActive("zoomOut"))
	{
		mCurrentZoom *= 1.2f;
		mView.zoom(1.2f);
		mWindow.setView(mView);
	}
    if (mActionMap.isActive("zoomReset"))
	{
		mCurrentZoom = 1.f;
		mView.setSize(mWindow.getSize().x * mView.getViewport().width, mWindow.getSize().y * mView.getViewport().height);
		mWindow.setView(mView);
	}
    if (mActionMap.isActive("resized"))
	{
		mView.setSize(mWindow.getSize().x * mView.getViewport().width, mWindow.getSize().y * mView.getViewport().height);
		mView.zoom(mCurrentZoom);
		mWindow.setView(mView);
	}

	// Gère le déplacement à la souris (clic molette)
	if (mActionMap.isActive("movingView"))
	{
		mView.move(- static_cast<float>(mCurrentMousePos.x - mLastMousePos.x) * mCurrentZoom,
						- static_cast<float>(mCurrentMousePos.y - mLastMousePos.y) * mCurrentZoom);
		mWindow.setView(mView);
	}
}

/// Appelé pour le rendu
void Game::OnRender()
{
	// Affichage
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
