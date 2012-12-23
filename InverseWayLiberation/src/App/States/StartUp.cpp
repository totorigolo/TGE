#include "StartUp.h"
#include "MainMenu.h"

// Ctor & Dtor
StartUp_State::StartUp_State()
{
}
StartUp_State::~StartUp_State()
{
}

// Exécute le State
State* StartUp_State::Run(App *app)
{
	// Change le titre de la fenêtre
	app->GetRenderWindow()->setTitle("Inverse Way Liberation - Lancement");

	// TODO: Faire un SplashScreen like :D

	return MainMenu_State::GetPInstance();
}