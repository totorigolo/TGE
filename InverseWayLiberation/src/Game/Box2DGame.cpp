#include "Box2DGame.h"
#include "../Entities/Player.h"
#include "../Level/LevelLoader.h"
#include "../Entities/BasicBody.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/Joints/MouseJoint.h"
#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Callback/PointCallback.h"
#include "../Tools/utils.h"
#include <iostream>

// Ctor
Box2DGame::Box2DGame(sf::RenderWindow & window)
	: mWindow(window), mQuit(false),
	// Ressources
	mResourceManager(ResourceManager::GetInstance()),
	mTextureMap(mResourceManager.GetTextureMap()),
	mInputManager(InputManager::GetInstance()),
	// Physique
	mPhysicMgr(PhysicManager::GetInstance()),
	// Level
	mLevel(LevelManager::GetInstance())
{
	// Etats du jeu
	mPaused = false;

	mMouseJointCreated = false;
	mMouseJointID = -1;
	mPinBodyA = nullptr;
	mPinBodyB = nullptr;
	mHookedSBody = nullptr;

	// Grapin de test
	mGrapnel = new Grapnel(-1);
	EntityManager::GetInstance().RegisterEntity(mGrapnel);
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
			// La boucle commence
			this->OnLoopBegin();

			// Appel des évènements
			this->OnEvent();
		
			// Appel de la logique
			this->OnLogic();

			// Gestion de la physique
			this->OnStepPhysics();

			// Appel des mises à jour
			this->OnUpdate();

			// Rendu
			this->OnRender();

			// La boucle se termine
			this->OnLoopEnd();
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

	// Initialise le monde
	mPhysicMgr.SetTimeStep(1.f / 60.f);
	mPhysicMgr.SetDebugDrawTarget(&mWindow);

	// Charge un niveau
	LevelLoader("lvls/1.xvl");
	if (!mLevel.IsCharged())
		return false;
	
	// Initialise la machine Lua
	mConsole.RegisterEntityFactory();
	mConsole.RegisterLevelManager();
	mConsole.RegisterGlobalLuaVar("level", &mLevel);
	mConsole.RegisterPhysicManager();
	mConsole.RegisterGlobalLuaVar("physicMgr", &mPhysicMgr);

	/* Evènements */
	// Enregistre la fênetre
	mInputManager.SetWindow(&mWindow);
	mWindow.setKeyRepeatEnabled(false);

	// Centre la vue
	mInputManager.SetView(mWindow.getDefaultView());
	mInputManager.SetDefaultZoom(mLevel.GetDefaultZoom());
	mInputManager.SetDefaultCenter(b22sfVec(mLevel.GetOriginView(), mPhysicMgr.GetPPM()));

	// Demande l'espionnage de touches
	mInputManager.AddSpyedKey(sf::Keyboard::T); // Ragdoll
	mInputManager.AddSpyedKey(sf::Keyboard::L); // Lamp
	mInputManager.AddSpyedKey(sf::Keyboard::R); // Reload
	mInputManager.AddSpyedKey(sf::Keyboard::P); // Pin
	mInputManager.AddSpyedKey(sf::Keyboard::H); // Hook
	mInputManager.AddSpyedKey(sf::Keyboard::I); // Console
	mInputManager.AddSpyedKey(sf::Keyboard::X); // test

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
	if (mInputManager.GetRMBState())
	{
		// Si la souris est déjà attachée, on met à jour la position
		if (mMouseJointCreated)
		{
			MouseJoint *j = ((MouseJoint*) mPhysicMgr.GetJoint(mMouseJointID));
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
					b2Body* body = callback.GetFixture()->GetBody();
					MouseJoint *j = new MouseJoint(body, staticBody, mMp, 10000000.f * body->GetMass());
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

	// Charge un niveau
	if (mInputManager.KeyReleased(sf::Keyboard::R))
	{
		// Supprime les pointeurs
		mGrapnel = nullptr;
		mHookedSBody = nullptr;
		mPinBodyA = nullptr;
		mPinBodyB = nullptr;

		// Charge le niveau
		LevelLoader("lvls/1.xvl");

		// Centre la vue
		mInputManager.SetDefaultZoom(mLevel.GetDefaultZoom());
		mInputManager.SetDefaultCenter(b22sfVec(mLevel.GetOriginView(), mPhysicMgr.GetPPM()));
	}

	// Grappin
	if (mInputManager.KeyPressed(sf::Keyboard::H))
	{
		// Si le grapin n'existe pas, on le crée
		if (!mGrapnel)
		{
			mGrapnel = new Grapnel(-1);
			EntityManager::GetInstance().RegisterEntity(mGrapnel);
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
				EntityFactory::LoadTexture("hook", "tex/hook.png");

				b2Body *b = callback.GetFixture()->GetBody();
				mGrapnel->Create(mTextureMap["hook"], b, b->GetLocalPoint(mMp), mHookedSBody, mHookedSAnchor);
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
			new DistanceJoint(mPinBodyA, mPinAnchorA, mPinBodyB, mPinAnchorB);

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
	mPhysicMgr.Step(10, 4);
	mPhysicMgr.GetWorld()->ClearForces();
	mPhysicMgr.DestroyBodiesOut(b2Vec2(1000.f, -200.f), b2Vec2(-200.f, 200.f));
}

/// Appelé pour les mises à jour
void Box2DGame::OnUpdate()
{
	// Met à jour les Joints
	mPhysicMgr.UpdateJoints();

	// Met à jour le niveau
	mLevel.Update();
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
	mPhysicMgr.DrawDebugData();
	
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

	// Arrête l'espionnage des touches
	mInputManager.RemoveSpyedKey(sf::Keyboard::T); // Ragdoll
	mInputManager.RemoveSpyedKey(sf::Keyboard::L); // Lamp
	mInputManager.RemoveSpyedKey(sf::Keyboard::R); // Reload
	mInputManager.RemoveSpyedKey(sf::Keyboard::P); // Pin
	mInputManager.RemoveSpyedKey(sf::Keyboard::H); // Hook
}
