#include "MainMenu.h"
#include "DevGame.h"

// Ctor & Dtor
MainMenu_State::MainMenu_State()
{
}
MainMenu_State::~MainMenu_State()
{
}

// Ex�cute le State
State* MainMenu_State::Run(App *app)
{
	// Change le titre de la fen�tre
	app->GetRenderWindow()->setTitle("Inverse Way Liberation - Menu principal");

	// TODO: Afficher un menu

	return DevGame_State::GetPInstance();
}