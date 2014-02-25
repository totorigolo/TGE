#include "stdafx.h"
#include "Game.h"
#include "../App/App.h"
#include "../App/InputManager.h"
#include "../Level/LevelLoader.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/EntityManager.h"

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
	mDebugDraw(false),
	// GUI
	mSfGUI(App::GetInstance().GetSfGUI())
{
	myAssert(mSfGUI, "La GUI n'a pas �t� cr��e.");
}

// Dtor
Game::~Game(void)
{
}

// (r�)Initialiser
void Game::Init()
{
	// Etats du jeu
	mPaused = false;
	mDebugDraw = false;
}

// Boucle de jeu
void Game::Run()
{
	// Appel l'initialisation
	if (this->OnInit())
	{
		// Tant que la fen�tre est ouverte
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

			// Appel des �v�nements
			this->OnEvent();

			if (focus)
			{
				// Appel de la logique
				this->OnLogic();

				// Gestion de la physique
				this->OnStepPhysics();

				// Appel des mises � jour
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

/* Fonctions �v�nements */
/// Initialise le jeu
bool Game::OnInit()
{
	// Initialise les variables
	mPaused = false;

	/* GUI */
	// Cr�e la Window et le Desktop
	mInputManager.AddDesktop(&mDesktop);
	mGUIElapsedTime.restart();

	// Charge le style de la GUI
	try {
		mDesktop.LoadThemeFromFile("data/style.css");
	}
	catch (const std::exception &e)
	{
		Dialog::Error("Erreur lors de la lecture du th�me :\n" + std::string(e.what()));
	}

	/* Physique */
	// Initialise le monde
	mPhysicMgr.SetTimeStep(1.f / 60.f);
	mPhysicMgr.SetDebugDrawTarget(&mWindow);

	/* Ev�nements */
	// Enregistre la vue
	mInputManager.SetView(mWindow.getDefaultView());

	// Demande l'espionnage de touches
	// Ev�nements
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::M)); // Pause physique
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::O)); // Debug Draw
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::R)); // Recharger

	// Initialise la machine Lua
	mConsole.RegisterEntityFactory();
	mConsole.RegisterLevelManager();
	mConsole.RegisterGlobalLuaVar("level", &mLevel);
	mConsole.RegisterPhysicManager();
	mConsole.RegisterGlobalLuaVar("physicMgr", &mPhysicMgr);
	mConsole.RegisterInputManager();
	mConsole.RegisterGlobalLuaVar("inputMgr", &mInputManager);
	mConsole.RegisterResourceManager();

	// Enregistre la console
	mLevel.SetLuaConsole(&mConsole);

	// Charge le niveau
	//Init();
	//mConsole.Reset();
	LevelLoader("lvls/level_1.xvl");

	return true;
}

/// Appel� quand la boucle commence
void Game::OnLoopBegin()
{
	// Sauvegarde la derni�re position de la souris
	mMp = sf2b2Vec(mInputManager.GetMousePosRV(), mPhysicMgr.GetMPP()); // syst�me Box2D
}

/// Appel� pour les �v�nements
void Game::OnEvent()
{
	// Met � jour les �v�nements
	mInputManager.Update();

	// V�rifie les �v�nements
	if (mInputManager.HasQuitted())
	{
		// Demande si on veut d�charger le niveau actuel
		if (1 == Dialog::ButtonChoice("Quitter le jeu ?",
									  "Voulez-vous quitter le jeu ?\nToute progression non sauvegard�e sera perdue.",
									  "Oui", "Non"))
									  mQuit = true;
	}
	if (!mInputManager.HasFocus())
	{
		return;
	}

	// Retient si la touche Control est enfonc�e
	bool ctrl = mInputManager.IsKeyPressed(sf::Keyboard::LControl) || mInputManager.IsKeyPressed(sf::Keyboard::RControl);

	// Pause physique
	if (mInputManager.KeyPressed(sf::Keyboard::M) && ctrl)
	{
		mPaused = !mPaused;
	}
	// DebugDraw
	if (mInputManager.KeyPressed(sf::Keyboard::O) && ctrl)
	{
		mDebugDraw = !mDebugDraw;
	}

	// Recharger le niveau
	if (mInputManager.KeyPressed(sf::Keyboard::R) && ctrl)
	{
		// Charge le niveau
		Init();
		mConsole.Reset();
		LevelLoader("lvls/level_1.xvl");

	}
}

/// Appel� pour la logique
void Game::OnLogic()
{
}

/// Appel� pour la physique
void Game::OnStepPhysics()
{
	// Simule
	if (!mPaused)
	{
		mPhysicMgr.Step(10, 4);

		mPhysicMgr.DestroyBodiesOut(b2Vec2(1000.f, -200.f), b2Vec2(-200.f, 200.f));
	}
}

/// Appel� pour les mises � jour
void Game::OnUpdate()
{
	// Met � jour les Joints
	mPhysicMgr.UpdateJoints();

	// Met � jour le niveau
	mLevel.Update();

	// Met � jour la GUI
	mDesktop.Update(mGUIElapsedTime.getElapsedTime().asSeconds());
	mGUIElapsedTime.restart();
}

/// Appel� pour le rendu
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
			f.loadFromFile("data/calibri.ttf"); // TODO: ResourceMgr
			fontLoaded = true;
		}
		sf::Text pause("Pause", f, 60U);
		sf::Vector2f pos(mInputManager.GetWindowView().getCenter().x - pause.getGlobalBounds().width / 2.f, 0.f);
		pos.y = mWindow.mapPixelToCoords(f2i(pos), mInputManager.GetWindowView()).y;
		pause.setPosition(pos);

		mWindow.setView(mInputManager.GetWindowView());
		mWindow.draw(pause);
		mWindow.setView(mInputManager.GetView());
	}

	// Affichage du tout
	mWindow.display();
}

/// Appel� quand la boucle se termine
void Game::OnLoopEnd()
{
}

/// Appel� quand le jeu se termine
void Game::OnQuit()
{
	Init();

	// Enl�ve le Desktop du InputManager
	mInputManager.RemoveDesktop(&mDesktop);

	// Supprime les SpyedKeys
	mSpyedKeys.clear();

	// Remet la vue par d�faut
	mWindow.setView(mWindow.getDefaultView());
	mInputManager.SetView(mWindow.getDefaultView());

	// Vide le niveau
	mLevel.Clear();

	// Vide
	// TODO: D�placer vers la LuaMachine
	mConsole.UnregisterGlobalLuaVar("level");
	mConsole.UnregisterGlobalLuaVar("physicMgr");
	mConsole.UnregisterGlobalLuaVar("inputMgr");
}
