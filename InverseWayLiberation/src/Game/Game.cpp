#include "Game.h"
#include "../App/App.h"
#include "../App/InputManager.h"
#include "../Entities/Grapnel.h"
#include "../Level/LevelSaver.h"
#include "../Level/LevelLoader.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/EntityManager.h"
#include "../Physics/Joints/MouseJoint.h"
#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Callback/PointCallback.h"
#include "../Tools/utils.h"

// Ctor
Game::Game(sf::RenderWindow &window)
	: mWindow(window), mQuit(false),
	// Ressources
	mResourceManager(ResourceManager::GetInstance()),
	mInputManager(InputManager::GetInstance()),
	// Physique
	mPhysicMgr(PhysicManager::GetInstance()),
	// Level
	mLevel(LevelManager::GetInstance()),
	// Etats du jeu
	mPaused(false),
	mDebugDraw(true),
	// GUI
	mSfGUI(App::GetInstance().GetSfGUI())
{
	myAssert(mSfGUI, "La GUI n'a pas été créée.");
}

// Dtor
Game::~Game(void)
{
}

// Boucle de jeu
void Game::Run()
{
	// Appel l'initialisation
	if (this->OnInit())
	{
		// Tant que la fenêtre est ouverte
		while (mWindow.isOpen() && !mQuit)
		{
			// Gestion du focus
			bool focus = mInputManager.HasFocus();
			if (!focus)
				mWindow.setFramerateLimit(20U);
			else
				mWindow.setFramerateLimit(60U);

			if (focus)
			{
				// La boucle commence
				this->OnLoopBegin();
			}

			// Appel des évènements
			this->OnEvent();

			if (focus)
			{
				// Appel de la logique
				this->OnLogic();

				// Gestion de la physique
				this->OnStepPhysics();

				// Appel des mises à jour
				this->OnUpdate();
			}

			// Rendu
			this->OnRender();

			if (focus)
			{
				// La boucle se termine
				this->OnLoopEnd();
			}
		}
	}

	// On quite
	this->OnQuit();
}

/* Fonctions évènements */
/// Initialise le jeu
bool Game::OnInit()
{
	// Initialise les variables
	mPaused = false;

	/* GUI */
	// Crée la Window et le Desktop
	mInputManager.AddDesktop(&mDesktop);
	mGUIElapsedTime.restart();

	/* Physique */
	// Initialise le monde
	mPhysicMgr.SetTimeStep(1.f / 60.f);
	mPhysicMgr.SetDebugDrawTarget(&mWindow);

	/* Evènements */
	// Enregistre la fênetre
	mInputManager.SetWindow(&mWindow);
	mWindow.setKeyRepeatEnabled(false);

	// Enregistre la vue
	mInputManager.SetView(mWindow.getDefaultView());

	// Demande l'espionnage de touches
	mInputManager.AddSpyedKey(sf::Keyboard::M); // Pause physique
	mInputManager.AddSpyedKey(sf::Keyboard::O); // Debug Draw
	mInputManager.AddSpyedKey(sf::Keyboard::T); // Ragdoll
	mInputManager.AddSpyedKey(sf::Keyboard::L); // Lamp
	mInputManager.AddSpyedKey(sf::Keyboard::R); // Reload
	mInputManager.AddSpyedKey(sf::Keyboard::S); // Save
	mInputManager.AddSpyedKey(sf::Keyboard::P); // Pin
	mInputManager.AddSpyedKey(sf::Keyboard::H); // Hook
	mInputManager.AddSpyedKey(sf::Keyboard::I); // Console
	mInputManager.AddSpyedKey(sf::Keyboard::X); // test

	// Initialise la machine Lua
	mConsole.RegisterEntityFactory();
	mConsole.RegisterLevelManager();
	mConsole.RegisterGlobalLuaVar("level", &mLevel);
	mConsole.RegisterPhysicManager();
	mConsole.RegisterGlobalLuaVar("physicMgr", &mPhysicMgr);
	mConsole.RegisterInputManager();
	mConsole.RegisterGlobalLuaVar("inputMgr", &mInputManager);
	mConsole.RegisterResourceManager();

	// Charge un niveau
	LevelLoader("lvls/1.xvl");
	if (!mLevel.IsCharged())
		return false;

	mLevel.SetLuaConsole(&mConsole);

	return true;
}

/// Appelé quand la boucle commence
void Game::OnLoopBegin()
{
	// Sauvegarde la dernière position de la souris
	mMp = sf2b2Vec(mInputManager.GetMousePosRV(), mPhysicMgr.GetMPP()); // système Box2D
}

/// Appelé pour les évènements
void Game::OnEvent()
{
	// Met à jour les évènements
	mInputManager.Update();

	// Vérifie les évènements
	if (mInputManager.HasQuitted())
		mQuit = true;
	if (!mInputManager.HasFocus())
		return;

	// DebugDraw
	if (mInputManager.KeyPressed(sf::Keyboard::O))
	{
		mDebugDraw = !mDebugDraw;
	}

	// Charge un niveau
	if (mInputManager.KeyReleased(sf::Keyboard::R))
	{
		// Réinitialise Lua et Triggers
		mConsole.Reset();

		// Charge le niveau
		if (!mInputManager.IsKeyPressed(sf::Keyboard::LControl))
			LevelLoader("lvls/1.xvl");
		else
			LevelLoader("lvls/save.xvl");
	}
}

/// Appelé pour la logique
void Game::OnLogic()
{
}

/// Appelé pour la physique
void Game::OnStepPhysics()
{
	// Simule
	if (!mPaused)
	{
		mPhysicMgr.Step(10, 4);
		mPhysicMgr.GetWorld()->ClearForces();
		mPhysicMgr.DestroyBodiesOut(b2Vec2(1000.f, -200.f), b2Vec2(-200.f, 200.f));
	}
}

/// Appelé pour les mises à jour
void Game::OnUpdate()
{
	// Met à jour les Joints
	mPhysicMgr.UpdateJoints();

	// Met à jour le niveau
	mLevel.Update();

	// Met à jour la GUI
	mDesktop.Update(mGUIElapsedTime.getElapsedTime().asSeconds());
	mGUIElapsedTime.restart();
}

/// Appelé pour le rendu
void Game::OnRender()
{
	// Rendu
	mWindow.clear(mLevel.GetBckgColor());
	mWindow.setView(mInputManager.GetView());
	
	// Affichage du Level
	mWindow.draw(mLevel);

	// Affichage du debug
	if (!mDebugDraw) mPhysicMgr.DrawDebugData();

	// Affichage de la GUI
	mSfGUI->Display(mWindow);

	// Si on n'a pas le focus
	if (!mInputManager.HasFocus())
	{
		static sf::Font f;
		static bool fontLoaded = false;
		if (!fontLoaded)
		{
			f.loadFromFile("tex/calibri.ttf"); // TODO: ResourceMgr
			fontLoaded = true;
		}
		sf::Text pause("Pause", f, 50U);
		pause.setPosition(mWindow.getSize().x / 2.f - pause.getGlobalBounds().width / 2.f, 0.f);

		mWindow.setView(mWindow.getDefaultView());
		mWindow.draw(pause);
		mWindow.setView(mInputManager.GetView());
	}

	// Affichage du tout
	mWindow.display();
}

/// Appelé quand la boucle se termine
void Game::OnLoopEnd()
{
}

/// Appelé quand le jeu se termine
void Game::OnQuit()
{
	// Enlève le Desktop du InputManager
	mInputManager.RemoveDesktop(&mDesktop);

	// Remet la vue par défaut
	mWindow.setView(mWindow.getDefaultView());

	// Vide le niveau
	mLevel.Clear();

	// Vide
	mConsole.UnregisterGlobalLuaVar("level");
	mConsole.UnregisterGlobalLuaVar("physicMgr");
	mConsole.UnregisterGlobalLuaVar("inputMgr");

	// Arrête l'espionnage des touches
	mInputManager.RemoveSpyedKey(sf::Keyboard::R); // Reload
}
