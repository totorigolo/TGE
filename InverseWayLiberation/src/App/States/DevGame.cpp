#include "DevGame.h"
#include "MainMenu.h"
#include "../../Game/Box2DGame.h"

// Ctor & Dtor
DevGame_State::DevGame_State()
{
}
DevGame_State::~DevGame_State()
{
}

// Exécute le State
State* DevGame_State::Run(App *app)
{
	// Obtient un pointeur vers la fenêtre
	sf::RenderWindow *window = app->GetRenderWindow();

	// Change le titre de la fenêtre
	window->setTitle("Inverse Way Liberation - DevGame");

	// Lance le jeu
	// TODO: Changer ça
	Box2DGame *game = new Box2DGame(*window);
	game->Run();
	delete game;

	return &MainMenu_State::GetInstance();
}
