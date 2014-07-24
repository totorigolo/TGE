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
	myAssert(mSfGUI, "La GUI n'a pas �t� cr��e.");
}

// Dtor
Editor::~Editor(void)
{
}

// (r�)Initialiser
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
		// Tant que la fen�tre est ouverte
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
bool Editor::OnInit()
{
	// Initialise les variables
	mPaused = false;

	/* GUI */
	// Cr�e la Window et le Desktop
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

	// Passe les objets � la LevelWindow
	mEditBox->GetLevelWindow()->SetEditor(this);
	mEditBox->GetLevelWindow()->SetEditBox(&*mEditBox);
	mEditBox->GetLevelWindow()->SetLuaMachine(&mConsole);

	// Charge les textures vides
	mResourceManager.LoadTexture("none", "tex/none.png");
	mResourceManager.LoadTexture("unknown", "tex/unknown.png");

	return true;
}

/// Appel� quand la boucle commence
void Editor::OnLoopBegin()
{
	// Sauvegarde la derni�re position de la souris
	mMp = sf2b2Vec(mInputManager.GetMousePosRV(), mPhysicMgr.GetMPP()); // syst�me Box2D
}

/// Appel� pour les �v�nements
void Editor::OnEvent()
{
	// Met � jour les �v�nements
	mInputManager.Update();

	// Ex�cute les actions planifi�es de la GUI
	mEditBox->DoScheduledTasks();

	// Retient si la touche Control est enfonc�e
	bool ctrl = mInputManager.IsKeyPressed(sf::Keyboard::LControl) || mInputManager.IsKeyPressed(sf::Keyboard::RControl);

	// V�rifie les �v�nements
	if (mInputManager.HasQuitted())
	{
		// Demande si on veut d�charger le niveau actuel
		if (1 == Dialog::ButtonChoice("Quitter l'�diteur ?",
									  "Voulez-vous quitter l'�diteur ?\nTout changement non sauvegard� sera perdu.",
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
				// Enregistre le body appuy�
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

			// Si on a cliqu� sur rien, on oublie les deux
			else
			{
				mPinBodyA = nullptr;
				mPinBodyB = nullptr;
			}
		}

		// Cr�e le joint
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
		// Si la fen�tre de cr�ation de polygones est en mode cr�ation, on transmet les clics
		if (mPolyCreationWindow->IsInEditMode())
		{
			mPolyCreationWindow->AddPoint(mMp);
		}
	}

	// EditBox : Deco Creation
	if (mInputManager.IsLMBClicked() && ctrl && mDecoCreationWindow)
	{
		// Si la fen�tre de cr�ation est en mode ajout, on transmet les clics
		if (mDecoCreationWindow->IsInAddMode())
		{
			mDecoCreationWindow->Add(mMp);
		}
	}

	// EditBox : BasicBody Creation
	if (mInputManager.IsLMBClicked() && ctrl && mBasicBodyCreationWindow)
	{
		// Si la fen�tre de cr�ation est en mode ajout, on transmet les clics
		if (mBasicBodyCreationWindow->IsInAddMode())
		{
			mBasicBodyCreationWindow->Add(mMp);
		}
	}

	// D�placements des objets
	if (!mPaused) // Physique en continue, on utilise le MouseJoint
	{
		if (mInputManager.GetRMBState()) // Cr�e ou met � jour le joint
		{
			// Si la souris est d�j� attach�e, on met � jour la position
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
						// R�cup�re le Body
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
	else // Physique en pause, on t�l�porte les objets
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
				// D�place le Body
				mMouseMovingBody->SetTransform(mMp - mMouseMovingBodyAnchor, mMouseMovingBody->GetAngle());
				mMouseMovingBody->SetAwake(true);
			}
		}
		else if (mMouseMovingBody) // Oublie le Body d�s qu'on relache le bouton
		{
			mMouseMovingBody = nullptr;
		}
	}
	
	// EditBox : S�lection des objets
	if (mMouseMovingBody)
	{
		myAssert(mMouseMovingBody->GetUserData(), "Un b2Body sans Entity a �t� s�lectionn�.");
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
			// R�cup�re le Body
			Entity *e = (Entity*) callback.GetFixture()->GetBody()->GetUserData();

			// Ajoute le Body � l'EditBox
			myAssert(e, "Un b2Body sans Entity a �t� s�lectionn�.");
			mEditBox->ChangeSelectedObject(e);
			found = true;
		}

		// Si on n'a pas de b2Body, on cherche ce qui n'est pas physique
		else
		{
			// R�cup�re la position de la souris en mode SFML
			sf::Vector2f mousePos = mInputManager.GetMousePosRV();

			// Les Entities (D�co)
			std::list<Entity*> &entities(mEntityMgr.GetEntities());
			for (auto it = entities.rbegin(); it != entities.rend(); ++it) // Par Layer
			{
				// Pour chaque type
				if ((*it)->GetType() == EntityType::Deco)
				{
					// V�rifie si le curseur est dessus
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

		// D�selectione le body courant si on n'en a pas trouv� de nouveau
		if (!found)
			mEditBox->Unselect();
	}

	// Gestion du zoom et d�placement de la vue
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

		// D�zoom
		else
			mInputManager.Zoom(1.2f);
	}
	if (mInputManager.GetMMBState())
	{
		mInputManager.MoveCenter(-i2f(mInputManager.GetMousePos() - mInputManager.GetLastMousePos()) * mInputManager.GetCurrentZoom());
	}
}

/// Appel� pour la logique
void Editor::OnLogic()
{
}

/// Appel� pour la physique
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

/// Appel� pour les mises � jour
void Editor::OnUpdate()
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

/// Appel� quand la boucle se termine
void Editor::OnLoopEnd()
{
}

/// Appel� quand le jeu se termine
void Editor::OnQuit()
{
	Init();

	// Enl�ve le Desktop du InputManager
	mInputManager.RemoveDesktop(&mDesktop);

	// Supprime l'EditBox
	mEditBox.reset();

	// Supprime les SpyedKeys
	mSpyedKeys.clear();

	// Remet la vue par d�faut
	mWindow.setView(mWindow.getDefaultView());
	mInputManager.SetView(mWindow.getDefaultView());

	// Supprime le joint de la souris
	mMouseJointCreated = false; // Suppr auto
	mMouseJointID = -1;

	// Vide le niveau
	mLevel.Clear();

	// Vide
	// TODO: D�placer vers la LuaMachine
	mConsole.UnregisterGlobalLuaVar("level");
	mConsole.UnregisterGlobalLuaVar("physicMgr");
	mConsole.UnregisterGlobalLuaVar("inputMgr");
}
