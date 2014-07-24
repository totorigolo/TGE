#include "stdafx.h"
#include "Editor.h"
#include "../App/App.h"
#include "../App/InputManager.h"
#include "../Level/LevelManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/Deco.h"
#include "../Entities/Player.h"
#include "../Entities/BasicBody.h"
#include "../Entities/EntityFactory.h"
#include "../Entities/EntityManager.h"
#include "../Entities/PartitioningTree.h"
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
	mDebugDraw(false),
	// GUI
	mSfGUI(App::GetInstance().GetSfGUI()),
	// Autre
	mPolyCreationWindow(nullptr),
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

// (ré)Initialiser
void Editor::Init()
{
	// Etats du jeu
	mPaused = false;
	mDebugDraw = false;
	// Autre
	mMouseMovingBody = nullptr;
	mMouseJointCreated = false;
	mMouseJointID = -1;
	mPinBodyA = nullptr;
	mPinBodyB = nullptr;
	mHookedSBody = nullptr;
	mGrapnel = nullptr;
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
				mWindow.setFramerateLimit(6000U);

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
	mDecoCreationWindow = mEditBox->GetDecoCreationWindow();
	mBasicBodyCreationWindow = mEditBox->GetBasicBodyCreationWindow();

	// Charge le style de la GUI
	try {
		mDesktop.LoadThemeFromFile("data/style.css");
	}
	catch (const std::exception &e)
	{
		Dialog::Error("Erreur lors de la lecture du thème :\n" + std::string(e.what()));
	}

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
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::T)); // Ragdoll
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::P)); // Pin
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::Add)); // Zoom in
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::Subtract)); // Zoom out
	mSpyedKeys.push_back(SpyedKey::Create(sf::Keyboard::Numpad0)); // Reset vue

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
	mEditBox->SetLuaMachine(&mConsole);
	mConsole.SetLuaConsole(mEditBox->GetLuaConsoleWindow());

	// Passe les objets à la LevelWindow
	mEditBox->GetLevelWindow()->SetEditor(this);
	mEditBox->GetLevelWindow()->SetEditBox(&*mEditBox);
	mEditBox->GetLevelWindow()->SetLuaMachine(&mConsole);

	// Charge les textures vides
	mResourceManager.LoadTexture("none", "tex/none.png");
	mResourceManager.LoadTexture("unknown", "tex/unknown.png");

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

	// Retient si la touche Control est enfoncée
	bool ctrl = mInputManager.IsKeyPressed(sf::Keyboard::LControl) || mInputManager.IsKeyPressed(sf::Keyboard::RControl);

	// Vérifie les évènements
	if (mInputManager.HasQuitted())
	{
		// Demande si on veut décharger le niveau actuel
		if (1 == Dialog::ButtonChoice("Quitter l'éditeur ?",
									  "Voulez-vous quitter l'éditeur ?\nTout changement non sauvegardé sera perdu.",
									  "Oui", "Non"))
			mQuit = true;
	}
	if (!mInputManager.HasFocus())
	{
		return;
	}

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

	// Epingle un objet
	if (mInputManager.KeyPressed(sf::Keyboard::P) && ctrl)
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

	// EditBox : Poly Creation
	if (mInputManager.IsLMBClicked() && ctrl && mPolyCreationWindow)
	{
		// Si la fenêtre de création de polygones est en mode création, on transmet les clics
		if (mPolyCreationWindow->IsInEditMode())
		{
			mPolyCreationWindow->AddPoint(mMp);
		}
	}

	// EditBox : Deco Creation
	if (mInputManager.IsLMBClicked() && ctrl && mDecoCreationWindow)
	{
		// Si la fenêtre de création est en mode ajout, on transmet les clics
		if (mDecoCreationWindow->IsInAddMode())
		{
			mDecoCreationWindow->Add(mMp);
		}
	}

	// EditBox : BasicBody Creation
	if (mInputManager.IsLMBClicked() && ctrl && mBasicBodyCreationWindow)
	{
		// Si la fenêtre de création est en mode ajout, on transmet les clics
		if (mBasicBodyCreationWindow->IsInAddMode())
		{
			mBasicBodyCreationWindow->Add(mMp);
		}
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
				PointCallback callback(mMp, false);
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

			// Les Entities (Déco)
			std::list<Entity*> &entities(mEntityMgr.GetEntities());
			for (auto it = entities.rbegin(); it != entities.rend(); ++it) // Par Layer
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

	// Gestion du zoom et déplacement de la vue
	if (mInputManager.KeyPressed(sf::Keyboard::Add) && ctrl)
	{
		mInputManager.Zoom(0.8f);
	}
	if (mInputManager.KeyPressed(sf::Keyboard::Subtract) && ctrl)
	{
		mInputManager.Zoom(1.2f);
	}
	if (mInputManager.KeyPressed(sf::Keyboard::Numpad0) && ctrl)
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

		b2Body *b = nullptr;
		Entity *e = mEditBox->GetSelectedEntity();
		if (e)
		{
			if (e->GetType() == EntityType::BasicBody || e->GetType() == EntityType::PolyBody)
				b = ((BaseBody*) e)->GetBody();
			else if (e->GetType() == EntityType::Player)
				b = ((Player*) e)->GetBody();
		}
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
	if (!mDebugDraw)
	{
		mPhysicMgr.DrawDebugData();
		PartitioningTree::GetInstance().DrawDebug(mWindow);
	}

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

/// Appelé quand la boucle se termine
void Editor::OnLoopEnd()
{
}

/// Appelé quand le jeu se termine
void Editor::OnQuit()
{
	Init();

	// Enlève le Desktop du InputManager
	mInputManager.RemoveDesktop(&mDesktop);

	// Supprime l'EditBox
	mEditBox.reset();

	// Supprime les SpyedKeys
	mSpyedKeys.clear();

	// Remet la vue par défaut
	mWindow.setView(mWindow.getDefaultView());
	mInputManager.SetView(mWindow.getDefaultView());

	// Supprime le joint de la souris
	mMouseJointCreated = false; // Suppr auto
	mMouseJointID = -1;

	// Vide le niveau
	mLevel.Clear();

	// Vide
	// TODO: Déplacer vers la LuaMachine
	mConsole.UnregisterGlobalLuaVar("level");
	mConsole.UnregisterGlobalLuaVar("physicMgr");
	mConsole.UnregisterGlobalLuaVar("inputMgr");
}
