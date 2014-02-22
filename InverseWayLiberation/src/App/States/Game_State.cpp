#include "stdafx.h"
#include "Game_State.h"
#include "MainMenu_State.h"
#include "../../Game/Game.h"

// Ctor & Dtor
Game_State::Game_State()
{
}
Game_State::~Game_State()
{
}

// Exécute le State
State* Game_State::Run(App *app)
{
	// Obtient un pointeur vers la fenêtre
	sf::RenderWindow *window = app->GetRenderWindow();
	window->setFramerateLimit(60U);

	// Change le titre de la fenêtre
	window->setTitle("Inverse Way Liberation - DevGame");

	// Lance le jeu
	Game *game = new Game(*window);
	game->Run();
	delete game;

	return &MainMenu_State::GetInstance();
}
