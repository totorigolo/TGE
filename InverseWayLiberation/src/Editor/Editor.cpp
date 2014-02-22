#include "stdafx.h"
#include "Editor.h"
#include "../App/App.h"
#include "../App/InputManager.h"
#include "../Entities/Grapnel.h"
#include "../Level/LevelSaver.h"
#include "../Level/LevelLoader.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/Deco.h"
#include "../Entities/BasicBody.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/EntityManager.h"
#include "../Physics/Joints/MouseJoint.h"
#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Callback/PointCallback.h"

// Ctor
Editor::Editor(sf::RenderWindow &window)
	: mWindow(window), mQuit(false),
	// Ressources
	mResourceManager(ResourceManager::GetInstance()),
	mInputManager(InputManager::GetInstance()),
	// Physique
	mPhysicMgr(PhysicManager::GetInstance()),
	// Level
	mLevel(LevelManager::GetInstance()),
	mEntityMgr(EntityManager::GetInstance()),
	// Etats du jeu
	mPaused(false),
	mDebugDraw(true),
	// GUI
	mSfGUI(App::GetInstance().GetSfGUI()),
	// Autre
	mPolyCreationWindow(nullptr),
	mPointJustAdded(false),
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
Editor::~Editor(void)
{
}

// Boucle de jeu
void Editor::Run()
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
bool Editor::OnInit()
{
	// Initialise les variables
	mPaused = false;

	/* GUI */
	// Crée la Window et le Desktop
	mInputManager.AddDesktop(&mDesktop);
	mGUIElapsedTime.restart();
	mEditBox = std::unique_ptr<EditBox>(new EditBox(mDesktop));
	mPolyCreationWindow = mEditBox->GetPolyCreationWindow();

	/* Physique */
	// Initialise le monde
	mPhysicMgr.SetTimeStep(1.f / 60.f);
	mPhysicMgr.SetDebugDrawTarget(&mWindow);

	/* Evènements */
	// Enregistre la vue
	mInputManager.SetView(mWindow.getDefaultView());

	// Demande l'espionnage de touches
	// Evènements
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::M)); // Pause physique
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::O)); // Debug Draw
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::B)); // Box
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::C)); // Circle
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::T)); // Ragdoll
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::L)); // Lamp
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::R)); // Reload
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::S)); // Save
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::P)); // Pin
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::H)); // Hook
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::I)); // Console
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::X)); // test

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

	// Charge la texture "vide"
	mResourceManager.LoadTexture("unknown", "tex/unknown.png");

	// Enregistre la console
	mLevel.SetLuaConsole(&mConsole);
	mEditBox->SetLuaMachine(&mConsole);

	return true;
}

/// Appelé quand la boucle commence
void Editor::OnLoopBegin()
{
	// Sauvegarde la dernière position de la souris
	mMp = sf2b2Vec(mInputManager.GetMousePosRV(), mPhysicMgr.GetMPP()); // système Box2D
}

/// Appelé pour les évènements
void Editor::OnEvent()
{
	// Met à jour les évènements
	mInputManager.Update();

	// Exécute les actions planifiées de la GUI
	mEditBox->DoScheduledTasks();

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
	if (mInputManager.KeyReleased(sf::Keyboard::B))
	{
		std::string list[] = {"box", "box2", "caisse", "way", "tonneau"};
		EntityFactory::CreateDynamicBox(getVec3(mMp), randomElement(list, 5));
	}
	if (mInputManager.KeyReleased(sf::Keyboard::C))
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

	// Poly Creation
	if (mInputManager.GetLMBState() && mInputManager.IsKeyPressed(sf::Keyboard::LControl) && mPolyCreationWindow && !mPointJustAdded)
	{
		// Si la fenêtre de création de polygones est en mode création, on gère les clics
		if (mPolyCreationWindow->IsInEditMode())
		{
			mPolyCreationWindow->AddPoint(mMp);
			mPointJustAdded = true;
		}
	}
	else if (mPointJustAdded && !mInputManager.GetLMBState())
	{
		// Attend le relâchement du clic pour mettre un autre point
		mPointJustAdded = false;
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
	if (!mPaused) // Physique en continue, on utilise le MouseJoint
	{
		if (mInputManager.GetRMBState()) // Crée ou met à jour le joint
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
						// Récupère le Body
						b2Body* body = callback.GetFixture()->GetBody();
						MouseJointDef def(body, staticBody, mMp, 100000000000.f * body->GetMass());
						MouseJoint *j = new MouseJoint(def);
						mMouseJointID = j->GetID();
						mMouseJointCreated = true;
						mMouseMovingBody = body;
					}
				}
			}
		}
		else if (mMouseJointCreated) // Supprime le joint
		{
			// Supprime le joint
			if (mPhysicMgr.JointExists(mMouseJointID))
			{
				mPhysicMgr.DestroyJoint(mMouseJointID);
				mMouseJointID = -1;
				mMouseJointCreated = false;
				mMouseMovingBody = nullptr;
			}
			// Le joint n'existe plus
			else
			{
				mMouseJointCreated = false;
				mMouseJointID = -1;
				mMouseMovingBody = nullptr;
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
	
	// EditBox : Sélection des objets
	if (mMouseMovingBody)
	{
		myAssert(mMouseMovingBody->GetUserData(), "Un b2Body sans Entity a été sélectionné.");
		mEditBox->ChangeSelectedObject((Entity*) mMouseMovingBody->GetUserData());
	}
	else if (mInputManager.GetRMBState())
	{
		bool found = false;

		// Demande au monde les formes qui sont sous l'AABB
		PointCallback callback(mMp, false);
		mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

		// Il y a un objet, on l'attache
		if (callback.GetFixture())
		{
			// Récupère le Body
			Entity *e = (Entity*) callback.GetFixture()->GetBody()->GetUserData();

			// Ajoute le Body à l'EditBox
			myAssert(e, "Un b2Body sans Entity a été sélectionné.");
			mEditBox->ChangeSelectedObject(e);
			found = true;
		}

		// Si on n'a pas de b2Body, on cherche ce qui n'est pas physique
		else
		{
			// Récupère la position de la souris en mode SFML
			sf::Vector2f mousePos = mInputManager.GetMousePosRV();

			// Les Entities (Déco, LivingBeing, Player)
			std::list<Entity*> &entities(mEntityMgr.GetEntities());
			for (auto it = entities.rbegin(); it != entities.rend(); ++it)
			{
				// Pour chaque type
				if ((*it)->GetType() == EntityType::Deco)
				{
					// Vérifie si le curseur est dessus
					Deco *d = (Deco*) (*it);
					if (d->GetSprite()->getGlobalBounds().contains(mousePos))
					{
						mEditBox->ChangeSelectedObject(d);
						found = true;
						break;
					}
				}
			}
		}

		// Déselectione le body courant si on n'en a pas trouvé de nouveau
		if (!found)
			mEditBox->Unselect();
	}

	// Charge et sauvegarde un niveau
	if (mInputManager.KeyReleased(sf::Keyboard::R))
	{
		// Déselectionne dans l'EditBox
		mEditBox->Unselect();

		// Supprime les pointeurs
		mGrapnel = nullptr;
		mHookedSBody = nullptr;
		mPinBodyA = nullptr;
		mPinBodyB = nullptr;

		// Réinitialise Lua
		mConsole.Reset();

		// Charge le niveau
		if (!mInputManager.IsKeyPressed(sf::Keyboard::LControl))
			LevelLoader("lvls/1.xvl");
		else
			LevelLoader("lvls/save.xvl");
	}
	if (mInputManager.KeyReleased(sf::Keyboard::S))
	{
		// Déselectionne dans l'EditBox
		mEditBox->Unselect();

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

	// Gestion du zoom et déplacement de la vue
	if (mInputManager.KeyPressed(sf::Keyboard::Add))
	{
		mInputManager.Zoom(0.8f);
	}
	if (mInputManager.KeyPressed(sf::Keyboard::Subtract))
	{
		mInputManager.Zoom(1.2f);
	}
	if (mInputManager.KeyPressed(sf::Keyboard::Numpad0))
	{
		mInputManager.SetZoom(mLevel.GetDefaultZoom());
		mInputManager.SetCenter(mLevel.GetDefaultCenter());
	}
	if (mInputManager.GetMouseWheelState())
	{
		// Zoom
		if (mInputManager.GetMouseWheelDelta() > 0)
			mInputManager.Zoom(0.8f);

		// Dézoom
		else
			mInputManager.Zoom(1.2f);
	}
	if (mInputManager.GetMMBState())
	{
		mInputManager.MoveCenter(-i2f(mInputManager.GetMousePos() - mInputManager.GetLastMousePos()) * mInputManager.GetCurrentZoom());
	}
}

/// Appelé pour la logique
void Editor::OnLogic()
{
}

/// Appelé pour la physique
void Editor::OnStepPhysics()
{
	// Simule
	if (!mPaused)
	{
		mPhysicMgr.Step(10, 4);
		mPhysicMgr.GetWorld()->ClearForces();

		b2Body *b = nullptr;
		Entity *e = mEditBox->GetSelectedEntity();
		if (e)
			if (e->GetType() == EntityType::BasicBody)
				b = ((BasicBody*) e)->GetBody();
		mPhysicMgr.DestroyBodiesOut(b2Vec2(1000.f, -200.f), b2Vec2(-200.f, 200.f), b);
	}
}

/// Appelé pour les mises à jour
void Editor::OnUpdate()
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
void Editor::OnRender()
{
	// Rendu
	mWindow.clear(mLevel.GetBckgColor());
	mWindow.setView(mInputManager.GetView());
	
	// Affichage du Level
	mWindow.draw(mLevel);

	// Affichage du debug
	if (!mDebugDraw) mPhysicMgr.DrawDebugData();

	// Affichage de la GUI
	if (mEditBox.get()) mWindow.draw(mEditBox->GetSelectionMark());
	if (mPolyCreationWindow) mPolyCreationWindow->DrawPointsOn(mWindow);
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
void Editor::OnLoopEnd()
{
}

/// Appelé quand le jeu se termine
void Editor::OnQuit()
{
	// Enlève le Desktop du InputManager
	mInputManager.RemoveDesktop(&mDesktop);

	// Remet la vue par défaut
	mWindow.setView(mWindow.getDefaultView());
	mInputManager.SetView(mWindow.getDefaultView());

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
}
