#include "Box2DGame.h"
#include "../Level/LevelLoader.h"
#include "../Entities/BasicBody.h"
#include "../Entities/Player.h"
#include "../Entities/EntityFactory.h"
#include "../Physics/Joints/DistanceJoint.h"
#include "../Physics/Callback/PointCallback.h"
#include "../Physics/Callback/FirstBodyRaycastCallback.h"
#include "../Tools/utils.h"
#include <iostream>
#include <vector>
#include <list>
#include <Thor/Animation.hpp>

// Ctor
Box2DGame::Box2DGame(sf::RenderWindow & window)
	: mWindow(window), mQuit(false),
	// Ressources
	mResourceManager(ResourceManager::GetInstance()),
	mTextureMap(mResourceManager.GetTextureMap()),
	mInputManager(InputManager::GetInstance()),
	// Physique
	mGravity(0.0f, -9.8f), mPhysicMgr(mGravity)
{
	// Etats du jeu
	mPaused = false;

	mMouseJointID = -1;
	mPinBodyA = nullptr;
	mPinBodyB = nullptr;
	mLevel = nullptr;
	mSpliceL1 = mSpliceL2 = b2Vec2(0.f, 0.f);
	mSpliceP1 = mSpliceP2 = b2Vec2(0.f, 0.f);
	mSplice1Get = false;
	mHookedSBody = nullptr;

	// Grapin de test
	mGrapnel = new Grapnel(&mPhysicMgr, -1);
	EntityManager::GetInstance().RegisterEntity(mGrapnel);
}

// Dtor
Box2DGame::~Box2DGame(void)
{
	delete mLevel;
	mLevel = nullptr;
}

// Boucle de jeu
void Box2DGame::Run()
{
	// Appel l'initialisation
	if (this->OnInit())
	{
		// Tant que la fen�tre est ouverte
		while (mWindow.isOpen() && !mQuit)
		{
			// La boucle commence
			this->OnLoopBegin();

			// Appel des �v�nements
			this->OnEvent();
		
			// Appel de la logique
			this->OnLogic();

			// Gestion de la physique
			this->OnStepPhysics();

			// Appel des mises � jour
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

/* Fonctions �v�nements */
/// Initialise le jeu
bool Box2DGame::OnInit()
{
	// Initialise les variables
	mPaused = false;

	// Initialise le monde
	mPhysicMgr.SetTimeStep(1.f / 60.f);
	mPhysicMgr.SetDebugDrawTarget(&mWindow);

	// Charge un niveau
	mLevel = new Level(&mPhysicMgr);
	LevelLoader("lvls/1.xvl", mLevel);
	if (!mLevel->IsCharged())
		return false;

	/* Fen�trage */
	// Initialise la renderTexture
	mRenderTexture.create(mWindow.getSize().x, mWindow.getSize().y);
	mShadowRenderTexture.create(mWindow.getSize().x, mWindow.getSize().y);

	/* Ev�nements */
	// Enregistre la f�netre
	mInputManager.SetWindow(&mWindow);
	mWindow.setKeyRepeatEnabled(false);

	// Centre la vue
	mInputManager.SetView(mWindow.getDefaultView());
	mInputManager.SetDefaultZoom(mLevel->GetDefaultZoom());
	mInputManager.SetDefaultCenter(b22sfVec(mLevel->GetOriginView(), mPhysicMgr.GetPPM()));

	// Demande l'espionnage de touches
	mInputManager.AddSpyedKey(sf::Keyboard::T); // Ragdoll
	mInputManager.AddSpyedKey(sf::Keyboard::L); // Lamp
	mInputManager.AddSpyedKey(sf::Keyboard::R); // Reload
	mInputManager.AddSpyedKey(sf::Keyboard::P); // Pin
	mInputManager.AddSpyedKey(sf::Keyboard::H); // Hook

	return true;
}

/// Appel� quand la boucle commence
void Box2DGame::OnLoopBegin()
{
	// Sauvegarde la derni�re position de la souris
	mMp = sf2b2Vec(mInputManager.GetMousePosRV(), mPhysicMgr.GetMPP()); // syst�me Box2D
}

/// Appel� pour les �v�nements
void Box2DGame::OnEvent()
{
	// Met � jour les �v�nements
	mInputManager.Update();

	// V�rifie les �v�nements
	if (mInputManager.HasQuitted())
		mQuit = true;

	// Cr�ation d'objets
	if (mInputManager.GetKeyState(sf::Keyboard::B))
	{
		std::string list[] = {"box", "box2", "caisse", "way", "tonneau"};
		EntityFactory::CreateBox(&mPhysicMgr, getVec3(mMp), list, 5);
	}
	if (mInputManager.GetKeyState(sf::Keyboard::C))
	{
		std::string list[] = {"ball", "circle"};
		EntityFactory::CreateCircle(&mPhysicMgr, getVec3(mMp), list, 2);
	}
	if (mInputManager.KeyReleased(sf::Keyboard::T))
	{
		EntityFactory::CreateRagdoll(&mPhysicMgr, mMp);
	}
	if (mInputManager.KeyReleased(sf::Keyboard::L))
	{
		EntityFactory::CreateLamp(&mPhysicMgr, getVec3(mMp), -1);
	}
	
	// D�placements des objets
	if (mInputManager.GetRMBState())
	{
		// Si la souris est d�j� attach�e, on met � jour la position
		MouseJoint *j = ((MouseJoint*) mPhysicMgr.GetJoint(mMouseJointID));
		if (j)
		{
			j->SetTarget(mMp);
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
					MouseJoint *j = new MouseJoint(&mPhysicMgr, body, staticBody, mMp, 10000000.f * body->GetMass());
					mMouseJointID = j->GetID();
				}
			}
		}
	}
	else if (mPhysicMgr.JointExists(mMouseJointID))
	{
		mPhysicMgr.DestroyJoint(mMouseJointID);
		mMouseJointID = -1;
	}

	// Charge un niveau
	if (mInputManager.KeyReleased(sf::Keyboard::R))
	{
		// Supprime les pointeurs
		mGrapnel->Destroy();
		if (mPhysicMgr.JointExists(mMouseJointID))
			mPhysicMgr.DestroyJoint(mMouseJointID);
		mMouseJointID = -1;
		mHookedSBody = nullptr;
		mPinBodyA = nullptr;
		mPinBodyB = nullptr;

		// Charge le niveau
		LevelLoader("lvls/1.xvl", mLevel);

		// Centre la vue
		mInputManager.SetDefaultZoom(mLevel->GetDefaultZoom());
		mInputManager.SetDefaultCenter(b22sfVec(mLevel->GetOriginView(), mPhysicMgr.GetPPM()));
		mRenderTextureView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mPhysicMgr.GetPPM()));
	}

	// Grappin
	if (mInputManager.KeyPressed(sf::Keyboard::H))
	{
		// Si le grapin n'existe pas, on le cr�e
		if (!mGrapnel)
		{
			mGrapnel = new Grapnel(&mPhysicMgr, -1);
			EntityManager::GetInstance().RegisterEntity(mGrapnel);
		}

		// Si le grapin est d�j� accroch�, on le d�croche
		if (mGrapnel->IsAlive())
			mGrapnel->Destroy();

		// Si le body est d�j� s�lectionn�, on l'accroche avec le grapin
		else if (mHookedSBody)
		{
			// Demande au monde les formes qui sont sous l'AABB
			PointCallback callback(mMp, false);
			mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

			// Il y a un objet, on le retient
			if (callback.GetFixture() && callback.GetFixture()->GetBody() != mHookedSBody)
			{
				b2Body *b = callback.GetFixture()->GetBody();
				mGrapnel->Create(mTextureMap["hook"], b, b->GetLocalPoint(mMp), mHookedSBody, mHookedSAnchor);
			}
			mHookedSBody = nullptr;
		}

		// Sinon on cherche le body survoll� pour l'accrocher
		else
		{
			// Demande au monde les formes qui sont sous l'AABB
			PointCallback callback(mMp, false);
			mPhysicMgr.GetWorld()->QueryAABB(&callback, callback.GetAABB());

			// Il y a un objet, on le retient
			if (callback.GetFixture())
			{
				// Enregistre le body appuy�
				mHookedSBody = callback.GetFixture()->GetBody();
				mHookedSAnchor = mHookedSBody->GetLocalPoint(mMp);
				//mHookedSAnchor = b2MulT(b2Rot(mHookedSBody->GetAngle()), mMp - mHookedSBody->GetPosition());
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
			new DistanceJoint(&mPhysicMgr, mPinBodyA, mPinAnchorA, mPinBodyB, mPinAnchorB);

			mPinBodyA = nullptr;
			mPinBodyB = nullptr;
		}
	}
}

/// Appel� pour la logique
void Box2DGame::OnLogic()
{
}

/// Appel� pour la physique
void Box2DGame::OnStepPhysics()
{
	mFrameTime.restart();

	// Simule
	mPhysicMgr.Step(10, 4);
	mPhysicMgr.GetWorld()->ClearForces();

	// Destruction des bodies en dehors de la zone
	b2Body *b = mPhysicMgr.GetBodyList(), *bb = nullptr;
	while (b)
	{
		// On supprime seulement les dynamicBodies
		if (b->GetType() == b2_dynamicBody)
		{
			bool erase = false;

			// V�rifie si l'objet est hors du monde
			b2Vec2 pos = b->GetPosition();
			if (pos.x < -200.f || pos.x > 200.f || pos.y < -200.f || pos.y > 1000.f)
			{
				erase = true;

				// V�rifie si l'objet n'est pas accroch� � la souris
				MouseJoint *j = (MouseJoint*) mPhysicMgr.GetJoint(mMouseJointID);
				if (j)
					if (b == j->GetAttachedBody())
						erase = false;
			}

			// Supprime l'Entity du body
			if (erase)
			{
				bb = b;
				b = b->GetNext();
				EntityManager::GetInstance().DestroyEntity((Entity*) bb->GetUserData());
			}

			// Sinon passe simplement au suivant
			else
				b = b->GetNext();
		}
		// Le body n'est pas un dynamicBody
		else
			b = b->GetNext();
	}
}

/// Appel� pour les mises � jour
void Box2DGame::OnUpdate()
{
	// Met � jour les Joints
	mPhysicMgr.UpdateJoints();

	// Met � jour le niveau
	mLevel->Update();
}

/// Appel� pour le rendu
void Box2DGame::OnRender()
{
	assert(mLevel && "est invalide.");

	// Rendu
	//mRenderTexture.clear(mLevel->GetBckgColor());
	mWindow.clear(mLevel->GetBckgColor());
	mWindow.setView(mInputManager.GetView());
	//mRenderTexture.setView(mRenderTextureView);
	
	// Affichage du Level
	//mRenderTexture.draw(*mLevel);
	mWindow.draw(*mLevel);
	
	// Affichage du debug
	mPhysicMgr.DrawDebugData();
	
	// Finalise le rendu des objets
	//mRenderTexture.display();
	//mWindow.draw(sf::Sprite(mRenderTexture.getTexture()));

	// Affichage du tout
	mWindow.display();
}

/// Appel� quand la boucle se termine
void Box2DGame::OnLoopEnd()
{
}

/// Appel� quand le jeu se termine
void Box2DGame::OnQuit()
{
	// Remet la vue par d�faut
	mWindow.setView(mWindow.getDefaultView());

	// Supprime le joint de la souris
	if (mPhysicMgr.JointExists(mMouseJointID))
	{
		mPhysicMgr.DestroyJoint(mMouseJointID);
	}
	mMouseJointID = -1;

	// Supprime le niveau
	delete mLevel;

	// Nullifie les pointeurs
	mLevel = nullptr;
	mGrapnel = nullptr;
	mHookedSBody = nullptr;
	mPinBodyA = nullptr;
	mPinBodyB = nullptr;

	// Arr�te l'espionnage des touches
	mInputManager.RemoveSpyedKey(sf::Keyboard::T); // Ragdoll
	mInputManager.RemoveSpyedKey(sf::Keyboard::L); // Lamp
	mInputManager.RemoveSpyedKey(sf::Keyboard::R); // Reload
	mInputManager.RemoveSpyedKey(sf::Keyboard::P); // Pin
	mInputManager.RemoveSpyedKey(sf::Keyboard::H); // Hook
}
