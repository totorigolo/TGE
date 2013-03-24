#include "App.h"
#include "../Sounds/SoundManager.h"
#include "States/StartUp.h"
#include "States/StateSystem.h"
#include <SFML/Graphics.hpp>
#include <iostream>

App::App()
	: mStateSystem(StateSystem::GetInstance()),
	mRenderWindow(nullptr)
{
}
App::~App()
{
	if (mRenderWindow)
	{
		delete mRenderWindow;
		mRenderWindow = nullptr;
	}
}

bool App::Init() 
{
	// TODO: Initialiser le son

	// Charge la fenêtre de rendu
	mRenderWindow = new sf::RenderWindow(sf::VideoMode(800U, 600U), "Inverse Way Liberation - Chargement");
	mRenderWindow->setFramerateLimit(60U);

	// Change le state
	mStateSystem.ChangeState(StartUp_State::GetPInstance());

	// Plante la graine
	srand(static_cast<unsigned int>(time(NULL)));

	// L'initialisation a réussi
    return true;
}

int App::Execute()
{
	// Initialise
    if (!Init())
	{
		std::cerr << "Une erreur est survenue lors de l'initialisation. Exiting..." << std::endl;
#ifdef _WIN32
		system("pause");
#endif

		return EXIT_FAILURE;
	}

	// Exécute les states
	while (mStateSystem.Run(this));

	// Nettoie
    CleanUp();

	// Tout s'est bien passé
    return EXIT_SUCCESS;
}

void App::CleanUp()
{
}

// Accesseurs
sf::RenderWindow* App::GetRenderWindow()
{
	return mRenderWindow;
}
