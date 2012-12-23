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
	// Change le titre de la fenêtre
	app->GetRenderWindow()->setTitle("Inverse Way Liberation - DevGame");

	// Lance le jeu
	// TODO: Changer ça
	Box2DGame game(*app->GetRenderWindow());
	game.Run();

	return Exit_State; // TODO: Changer ici quand il y aura un menu
	return MainMenu_State::GetPInstance();
}
