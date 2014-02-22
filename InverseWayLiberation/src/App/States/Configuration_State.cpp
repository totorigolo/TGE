#include "stdafx.h"
#include "Configuration_State.h"
#include "MainMenu_State.h"

// Ctor & Dtor
Configuration_State::Configuration_State()
	: mInputManager(InputManager::GetInstance()),
	mSfGUI(App::GetInstance().GetSfGUI())
{
}
Configuration_State::~Configuration_State()
{
}

// Exécute le State
State* Configuration_State::Run(App *app)
{
	// Récupère la fenêtre
	sf::RenderWindow *window = app->GetRenderWindow();
	window->setFramerateLimit(30U);

	// Change le titre de la fenêtre
	window->setTitle("Inverse Way Liberation - Configuration");

	// Met en place la GUI
	CreateGUI();

	// Boucle d'affichage
	mQuit = false;
	while (window->isOpen() && !mQuit)
	{
		// Evènements
		mInputManager.Update();
		if (mInputManager.HasQuitted())
		{
			OnExit();
		}

		// Met à jour la GUI
		mDesktop.Update(mGUIElapsedTime.getElapsedTime().asSeconds());
		mGUIElapsedTime.restart();

		// Affichage
		window->clear();
		window->setView(mInputManager.GetView());
		mSfGUI->Display(*window);
		window->display();
	}
	
	return &MainMenu_State::GetInstance();
}

// Gestion de la GUI
void Configuration_State::CreateGUI()
{
	// Crée la Window et le Desktop une seule fois
	static bool once = false;
	if (!once)
	{
		mInputManager.AddDesktop(&mDesktop);
		mGUIElapsedTime.restart();
		once = true;
	}

	// Crée la fenêtre
	mWindow = sfg::Window::Create();
	mDesktop.Add(mWindow);

	// Initialise les éléments
	mVBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	mLabel = sfg::Label::Create("Il n'y a aucune configuration pour le moment.\n\n\nRepassez plus tard :)");
	mVBox->PackEnd(mLabel);
	mExit = sfg::Button::Create("Fermer");
	mExit->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Configuration_State::OnExit, this));
	mVBox->PackEnd(mExit);
	mWindow->Add(mVBox);
}
void Configuration_State::EmptyGUI()
{
	if (mWindow) mDesktop.Remove(mWindow);
	mVBox.reset();
	mWindow.reset();
	mLabel.reset();
	mExit.reset();
}

// Callbacks
void Configuration_State::OnExit()
{
	EmptyGUI();
	mQuit = true;
}
