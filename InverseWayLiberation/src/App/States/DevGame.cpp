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

// Ex�cute le State
State* DevGame_State::Run(App *app)
{
	// Obtient un pointeur vers la fen�tre
	sf::RenderWindow *window = app->GetRenderWindow();

	// Change le titre de la fen�tre
	window->setTitle("Inverse Way Liberation - DevGame");

	// Lance le jeu
	// TODO: Changer �a
	Box2DGame *game = new Box2DGame(*window);
	game->Run();
	delete game;

	return &MainMenu_State::GetInstance();
}
