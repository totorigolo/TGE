#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Events.hpp>

class Game
{
public:
	/// Ctor & dtor
	Game(sf::RenderWindow & window);
	virtual ~Game(void);

	/// Boucle de jeu
	void Run();

protected:
	/* Evènements du jeu */
	/// Initialise le jeu
	virtual void OnInit();

	/// Appelé quand la boucle commence
	virtual void OnLoopBegin();

	/// Appelé pour les évènements
	virtual void OnEvent();

	/// Appelé pour le rendu
	virtual void OnRender(); // = 0; <- Pour la transformer en classe abstraite

	/// Appelé quand la boucle se termine
	virtual void OnLoopEnd();

	/// Appelé quand le jeu se termine
	virtual void OnQuit();
	
	/* Pour la molette de la souris */
	friend class OnMouseWheelMoved;

protected:
	// Etats du jeu
	bool mPaused;
	bool mExited;

	// Fenêtre
	sf::RenderWindow & mWindow;
	sf::View mView;
	float mCurrentZoom;
	
	// Evènements
	thor::ActionMap<std::string> mActionMap;
	thor::ActionMap<std::string>::CallbackSystem mActionCallbackSystem;

	// Positions de la souris
	sf::Vector2f mCurrentMousePos;
	sf::Vector2f mCurrentMousePosRV; // RV = relative to view
	sf::Vector2f mLastMousePos;
	sf::Vector2f mLastMousePosRV; // RV = relative to view

	// Simulation physique
	sf::Clock mElapsedTime;
};

/* Pour la molette de la souris */
class OnMouseWheelMoved
{
public:
	explicit OnMouseWheelMoved(Game *_game);
	void operator() (thor::ActionContext<std::string> context);

private:
	Game *mGame;
};
