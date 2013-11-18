#include "Box2DGame.h"
#include "LuaMachine.h"
#include "InputManager.h"
#include "../App/App.h"
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
Box2DGame::Box2DGame(sf::RenderWindow &window)
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
	mSfGUI(App::GetInstance().GetSfGUI()),
	// Autre
	mMouseMovingBody(nullptr),
	mMouseJointCreated(false),
	mMouseJointID(-1),
	mPinBodyA(nullptr),
	mPinBodyB(nullptr),
	mHookedSBody(nullptr),
	mGrapnel(nullptr)
{
	myAssert(mSfGUI, "La GUI n'a pas été créée.");
}

// Dtor
Box2DGame::~Box2DGame(void)
{
	mLevel.Clear();
}

// Boucle de jeu
void Box2DGame::Run()
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
bool Box2DGame::OnInit()
{
	// Initialise les variables
	mPaused = false;

	/* GUI */
	// Crée la Window et le Desktop
	mInputManager.AddDesktop(&mDesktop);
	mTestWindow = sfg::Window::Create();
	mTestWindow->SetTitle("Test Window");
	mDesktop.Add(mTestWindow);
	mGUIElapsedTime.restart();

	// Crée les éléments
	mButton = sfg::Button::Create("Hello !!!");
	mTestWindow->Add(mButton);

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
void Box2DGame::OnLoopBegin()
{
	// Sauvegarde la dernière position de la souris
	mMp = sf2b2Vec(mInputManager.GetMousePosRV(), mPhysicMgr.GetMPP()); // système Box2D
}

/// Appelé pour les évènements
void Box2DGame::OnEvent()
{
	// Met à jour les évènements
	mInputManager.Update();

	// Vérifie les évènements
	if (mInputManager.HasQuitted())
		mQuit = true;
	if (!mInputManager.HasFocus())
		return;

	// Pause physique
	if (mInputManager.KeyPressed(sf::Keyboard::M))
	{
		mPaused = !mPaused;
	}
	// DebugDraw
	if (mInputManager.KeyPressed(sf::Keyboard::O))
	{
		mDebugDraw = !mDebugDraw;
	}

	// Création d'objets
	if (mInputManager.IsKeyPressed(sf::Keyboard::B))
	{
		std::string list[] = {"box", "box2", "caisse", "way", "tonneau"};
		EntityFactory::CreateDynamicBox(getVec3(mMp), randomElement(list, 5));
	}
	if (mInputManager.IsKeyPressed(sf::Keyboard::C))
	{
		std::string list[] = {"ball", "circle"};
		EntityFactory::CreateDynamicCircle(getVec3(mMp), randomElement(list, 2));
	}
	if (mInputManager.KeyReleased(sf::Keyboard::T))
	{
		EntityFactory::CreateRagdoll(mMp);
	}
	if (mInputManager.KeyReleased(sf::Keyboard::L))
	{
		EntityFactory::CreateLamp(getVec3(mMp), -1);
	}

	// Lua
	if (mInputManager.KeyPressed(sf::Keyboard::I))
	{
		mConsole.DoFile("scripts/test1.lua");
	}
	if (mInputManager.KeyPressed(sf::Keyboard::X))
	{
		mConsole.DoFile("scripts/lvl1.lua");
	}

	// Déplacements des objets
	if (!mPaused && !mMouseMovingBody) // Physique en continue, on utilise le MouseJoint
	{
		if (mInputManager.GetRMBState())
		{
			// Si la souris est déjà attachée, on met à jour la position
			if (mMouseJointCreated)
			{
				MouseJoint *j = ((MouseJoint*)mPhysicMgr.GetJoint(mMouseJointID));
				if (j)
				{
					j->SetTarget(mMp);
				}
				// Le joint n'existe plus
				else
				{
					mMouseJointCreated = false;
					mMouseJointID = -1;
				}
			}

			// Sinon on recherche l'objet sous la souris et on l'attache
			else
			{
				// Demande au monde les formes qui sont sous l'AABB
				PointCallback callback(mMp);
				mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

				// Il y a un objet, on l'attache
				if (callback.GetFixture())
				{
					// On cherche un StaticBody disponible
					b2Body *staticBody = mPhysicMgr.GetAnyStaticBody();
					if (staticBody) // Si il y en a un
					{
						// Récupère le Body
						b2Body* body = callback.GetFixture()->GetBody();
						MouseJointDef def(body, staticBody, mMp, 100000000000.f * body->GetMass());
						MouseJoint *j = new MouseJoint(def);
						mMouseJointID = j->GetID();
						mMouseJointCreated = true;
					}
				}
			}
		}
		else if (mMouseJointCreated)
		{
			if (mPhysicMgr.JointExists(mMouseJointID))
			{
				mPhysicMgr.DestroyJoint(mMouseJointID);
				mMouseJointID = -1;
			}
			// Le joint n'existe plus
			else
			{
				mMouseJointCreated = false;
				mMouseJointID = -1;
			}
		}
	}
	else // Physique en pause, on téléporte les objets
	{
		// Si on appuie sur le clic droit, et tant qu'on appuie dessus
		if (mInputManager.GetRMBState())
		{
			if (!mMouseMovingBody) // Si on n'a pas encore d'objet, on le cherche
			{
				// Demande au monde les formes qui sont sous l'AABB
				PointCallback callback(mMp);
				mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

				// Il y a un objet, on l'attache
				if (callback.GetFixture())
				{
					mMouseMovingBody = callback.GetFixture()->GetBody();
					mMouseMovingBodyAnchor = mMp - mMouseMovingBody->GetWorldCenter();
				}
			}
			if (mMouseMovingBody)
			{
				// Déplace le Body
				mMouseMovingBody->SetTransform(mMp - mMouseMovingBodyAnchor, mMouseMovingBody->GetAngle());
				mMouseMovingBody->SetAwake(true);
			}
		}
		else if (mMouseMovingBody) // Oublie le Body dès qu'on relache le bouton
		{
			mMouseMovingBody = nullptr;
		}
	}
	
	// Charge et sauvegarde un niveau
	if (mInputManager.KeyReleased(sf::Keyboard::R))
	{
		// Supprime les pointeurs
		mGrapnel = nullptr;
		mHookedSBody = nullptr;
		mPinBodyA = nullptr;
		mPinBodyB = nullptr;

		// Réinitialise Lua et Triggers
		mConsole.Reset();
		mLevel.GetTriggersMgr().Clear();

		// Charge le niveau
		if (!mInputManager.IsKeyPressed(sf::Keyboard::LControl))
			LevelLoader("lvls/1.xvl");
		else
			LevelLoader("lvls/save.xvl");
	}
	if (mInputManager.KeyReleased(sf::Keyboard::S))
	{
		// Sauvegarde le niveau
		LevelSaver(mLevel, "lvls/save.xvl");
	}

	// Grappin
	if (mInputManager.KeyPressed(sf::Keyboard::H))
	{
		// Si le grapin n'existe pas, on le crée
		if (!mGrapnel)
		{
			mGrapnel = new Grapnel(-1);
		}

		// Si le grapin est déjà accroché, on le décroche
		if (mGrapnel->IsAlive())
			mGrapnel->Destroy();

		// Si le body est déjà sélectionné, on l'accroche avec le grapin
		else if (mHookedSBody)
		{
			// Demande au monde les formes qui sont sous l'AABB
			PointCallback callback(mMp, false);
			mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

			// Il y a un objet, on le retient
			if (callback.GetFixture() && callback.GetFixture()->GetBody() != mHookedSBody)
			{
				mResourceManager.LoadTexture("hook", "tex/hook.png");

				b2Body *b = callback.GetFixture()->GetBody();
				mGrapnel->Create(mResourceManager.GetTexture("hook"), b, b->GetLocalPoint(mMp), mHookedSBody, mHookedSAnchor);
			}
			mHookedSBody = nullptr;
		}

		// Sinon on cherche le body survollé pour l'accrocher
		else
		{
			// Demande au monde les formes qui sont sous l'AABB
			PointCallback callback(mMp, false);
			mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

			// Il y a un objet, on le retient
			if (callback.GetFixture())
			{
				// Enregistre le body appuyé
				mHookedSBody = callback.GetFixture()->GetBody();
				mHookedSAnchor = mHookedSBody->GetLocalPoint(mMp);
			}
		}
	}

	// Epingle un objet
	if (mInputManager.KeyPressed(sf::Keyboard::P))
	{
		// Enregistre les bobies et leurs ancres
		if (!mPinBodyA || !mPinBodyB)
		{
			// Demande au monde les formes qui sont sous l'AABB
			PointCallback callback(mMp, false);
			mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

			// Il y a un objet, on le retient
			if (callback.GetFixture())
			{
				// Enregistre le body appuyé
				if (!mPinBodyA && mPinBodyB != callback.GetFixture()->GetBody())
				{
					mPinBodyA = callback.GetFixture()->GetBody();
					mPinAnchorA = mPinBodyA->GetLocalPoint(mMp);
				}
				else if (!mPinBodyB && mPinBodyA != callback.GetFixture()->GetBody())
				{
					mPinBodyB = callback.GetFixture()->GetBody();
					mPinAnchorB = mPinBodyB->GetLocalPoint(mMp);
				}
				else
				{
					mPinBodyA = nullptr;
					mPinBodyB = nullptr;
				}
			}

			// Si on a cliqué sur rien, on oublie les deux
			else
			{
				mPinBodyA = nullptr;
				mPinBodyB = nullptr;
			}
		}

		// Crée le joint
		if (mPinBodyA && mPinBodyB)
		{
			DistanceJointDef def(mPinBodyA, mPinAnchorA, mPinBodyB, mPinAnchorB);
			new DistanceJoint(def);

			mPinBodyA = nullptr;
			mPinBodyB = nullptr;
		}
	}
}

/// Appelé pour la logique
void Box2DGame::OnLogic()
{
}

/// Appelé pour la physique
void Box2DGame::OnStepPhysics()
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
void Box2DGame::OnUpdate()
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
void Box2DGame::OnRender()
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
void Box2DGame::OnLoopEnd()
{
}

/// Appelé quand le jeu se termine
void Box2DGame::OnQuit()
{
	// Enlève le Desktop du InputManager
	mInputManager.RemoveDesktop(&mDesktop);

	// Remet la vue par défaut
	mWindow.setView(mWindow.getDefaultView());

	// Supprime le joint de la souris
	mMouseJointCreated = false; // Suppr auto
	mMouseJointID = -1;

	// Vide le niveau
	mLevel.Clear();

	// Nullifie les pointeurs
	mGrapnel = nullptr;
	mHookedSBody = nullptr;
	mPinBodyA = nullptr;
	mPinBodyB = nullptr;

	// Vide
	mConsole.UnregisterGlobalLuaVar("level");
	mConsole.UnregisterGlobalLuaVar("physicMgr");
	mConsole.UnregisterGlobalLuaVar("inputMgr");

	// Arrête l'espionnage des touches
	mInputManager.RemoveSpyedKey(sf::Keyboard::T); // Ragdoll
	mInputManager.RemoveSpyedKey(sf::Keyboard::L); // Lamp
	mInputManager.RemoveSpyedKey(sf::Keyboard::R); // Reload
	mInputManager.RemoveSpyedKey(sf::Keyboard::P); // Pin
	mInputManager.RemoveSpyedKey(sf::Keyboard::H); // Hook
}
