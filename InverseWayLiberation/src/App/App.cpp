#include "stdafx.h"
#include "App.h"
#include "../Sounds/SoundManager.h"
#include "States/StateSystem.h"
#include "States/StartUp_State.h"

App::App()
	: mStateSystem(StateSystem::GetInstance()),
	mRenderWindow(nullptr), mInputManager(InputManager::GetInstance()), mSfGUI(nullptr)
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
	mRenderWindow->setKeyRepeatEnabled(false);

	// Enregistre la fen�tre dans l'InputManager
	mInputManager.SetWindow(mRenderWindow);
	mInputManager.SetView(mRenderWindow->getDefaultView());

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
