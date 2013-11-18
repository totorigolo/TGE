#include "App.h"
#include "../Sounds/SoundManager.h"
#include "States/StateSystem.h"
#include "States/StartUp.h"

#include <SFML/Graphics.hpp>
#include <iostream>

App::App()
	: mStateSystem(StateSystem::GetInstance()),
	mRenderWindow(nullptr), mSfGUI(nullptr)
{
}
App::~App()
{
	// Suppprime la fen�tre de rendu
	if (mRenderWindow)
	{
		delete mRenderWindow;
		mRenderWindow = nullptr;
	}
}

bool App::Init() 
{
	// TODO: Initialiser le son

	// Cr�e la GUI
	mSfGUI = new sfg::SFGUI;

	// Charge la fen�tre de rendu
	mRenderWindow = new sf::RenderWindow(sf::VideoMode(800U, 600U), "Inverse Way Liberation - Chargement");
	mRenderWindow->setFramerateLimit(60U);

	// Change le state
	mStateSystem.ChangeState(&StartUp_State::GetInstance());

	// Plante la graine
	srand(static_cast<unsigned int>(time(NULL)));

	// L'initialisation a r�ussi
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

	// Ex�cute les states
	while (mStateSystem.Run(this));

	// Nettoie
    CleanUp();

	// Tout s'est bien pass�
    return EXIT_SUCCESS;
}

void App::CleanUp()
{
	// Suppprime la GUI
	if (mSfGUI)
	{
		delete mSfGUI;
		mSfGUI = nullptr;
	}
}

// Accesseurs
sfg::SFGUI* App::GetSfGUI()
{
	return mSfGUI;
}
sf::RenderWindow* App::GetRenderWindow()
{
	return mRenderWindow;
}
