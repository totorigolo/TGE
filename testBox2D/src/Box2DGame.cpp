#include "Box2DGame.h"
#include "Physics/config.h"
#include "Physics/StaticBox.h"
#include "Physics/DynamicBox.h"
#include "Physics/DynamicCircle.h"
#include "Physics/DistanceJoint.h"
#include "Physics/OverlappingBodyCallback.h"
#include "utils.h"
#include <iostream>

// Ctor
Box2DGame::Box2DGame(sf::RenderWindow & window)
	: Game(window),
	// Physique
	mGravity(0.0f, -9.8f), mWorld(mGravity)
{
	mMouseJoint = nullptr;
	mPinBodyA = nullptr;
	mPinBodyB = nullptr;
}

// Dtor
Box2DGame::~Box2DGame(void)
{
}

/* Surcharge des fonctions évènements */
/// Initialise le jeu
void Box2DGame::OnInit()
{
	Game::OnInit();

	// Centre la vue
	mView.setCenter(sf::Vector2f(0.f, 0.f));
	mWindow.setView(mView);

	// Charge les textures dans la textureKeyMap
	try {
		mTextureMap["box"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/box.png"));
		mTextureMap["box2"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/box2.png"));
		mTextureMap["ground"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/ground.png"));
		mTextureMap["circle"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/circle.png"));
	}
	catch (thor::ResourceLoadingException const& e)
	{
		std::cout << e.what() << std::endl;
		system("PAUSE");
		exit(222);
	}

	// Crée le sol
	mWorld.RegisterBody(new StaticBox(&mWorld, b2Vec2(0.f, -6.f), mTextureMap["ground"]));

	/* Crée les actions */
	mActionMap["onMoveObject"] = thor::Action(sf::Mouse::Right, thor::Action::Hold);
	mActionMap["onCreateBox"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);
	mActionMap["onCreateCircle"] = thor::Action(sf::Keyboard::C, thor::Action::Hold);
	mActionMap["onPin"] = thor::Action(sf::Keyboard::P, thor::Action::PressOnce);
}

/// Appelé quand la boucle commence
void Box2DGame::OnLoopBegin()
{
	Game::OnLoopBegin();

	// Converti la position de la souris en système Box2D
	mMp.x = mCurrentMousePosRV.x * MPP;
	mMp.y = - mCurrentMousePosRV.y * MPP;

	// Simule
	mWorld.Step(1.f / 60.f, 8, 3);

	// Affichage des objets dynamiques
	for (auto it = mWorld.GetBodyList().begin(); it != mWorld.GetBodyList().end(); )
	{
		bool erase = false;
		// Vérifie si l'objet est hors du monde et pas accroché à la souris
		if (!(*it)->IsInRange(b2Vec2(200.f, -200.f), b2Vec2(800.f, -200.f)))
			{
				erase = true;
				if (mMouseJoint)
					if (*it == mMouseJoint->GetAttachedBody())
						erase = false;
			}

		// Supprime le body
		if (erase)
		{
			auto it2 = it;
			++it2;
			mWorld.DestroyBody(*it);
			it = it2;
		}

		// Sinon passe simplement au suivant
		else
		{
			++it;
		}
	}
}

/// Appelé pour les évènements
void Box2DGame::OnEvent()
{
	Game::OnEvent();

	// Création d'objets
	if (mActionMap.isActive("onCreateBox"))
	{
		std::string list[] = {"box", "box2"};
		mWorld.RegisterBody(new DynamicBox(&mWorld, mMp, mTextureMap[randomElement(list, 2)]));
	}
	if (mActionMap.isActive("onCreateCircle"))
	{
		mWorld.RegisterBody(new DynamicCircle(&mWorld, mMp, mTextureMap["circle"]));
	}

	// Epingle un objet
	if (mActionMap.isActive("onPin"))
	{
		if (!mPinBodyA || !mPinBodyB)
		{
			// Crée une petite AABB sur la souris
			b2AABB aabb;
			b2Vec2 d;
			d.Set(0.001f, 0.001f);
			aabb.lowerBound = mMp - d;
			aabb.upperBound = mMp + d;

			// Demande au monde les formes qui sont sous l'AABB
			OverlappingBodyCallback callback(mMp);
			mWorld.QueryAABB(&callback, aabb);

			// Il y a un objet, on le retient
			if (callback.GetFixture())
			{
				if (!mPinBodyA && mPinBodyB != (Body*) callback.GetFixture()->GetBody()->GetUserData())
				{
					mPinBodyA = (Body*) callback.GetFixture()->GetBody()->GetUserData();
					mPinAnchorA = mMp;
				}
				else if (!mPinBodyB && mPinBodyA != (Body*) callback.GetFixture()->GetBody()->GetUserData())
				{
					mPinBodyB = (Body*) callback.GetFixture()->GetBody()->GetUserData();
					mPinAnchorB = mMp;
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
		if (mPinBodyA && mPinBodyB)
		{
			mWorld.RegisterJoint(new DistanceJoint(&mWorld, mPinBodyA, mPinAnchorA, mPinBodyB, mPinAnchorB));

			mPinBodyA = nullptr;
			mPinBodyB = nullptr; // TODO : mPinAchor => local
		}
	}

	// Déplacements des objets
	if (mActionMap.isActive("onMoveObject"))
	{
		// Si la souris est déjà attachée, on met à jour la position
		if (mMouseJoint)
		{
			mMouseJoint->SetTarget(mMp);
		}

		// Sinon on recherche l'objet sous la souris et on l'attache
		else
		{
			// Crée une petite AABB sur la souris
			b2AABB aabb;
			b2Vec2 d;
			d.Set(0.001f, 0.001f);
			aabb.lowerBound = mMp - d;
			aabb.upperBound = mMp + d;

			// Demande au monde les formes qui sont sous l'AABB
			OverlappingBodyCallback callback(mMp);
			mWorld.QueryAABB(&callback, aabb);

			// Il y a un objet, on l'attache
			if (callback.GetFixture())
			{
				b2Body* b2body = callback.GetFixture()->GetBody();
				Body* body = (Body*) b2body->GetUserData();
				mMouseJoint = new MouseJoint(&mWorld, body, mWorld.GetAnyStaticBody(), mMp, 80000.f * b2body->GetMass());
			}
		}
	}
	else
	{
		if (mMouseJoint)
			mWorld.DestroyJoint(mMouseJoint);
		mMouseJoint = nullptr;
	}
}

/// Appelé pour le rendu
void Box2DGame::OnRender()
{
	mWindow.clear(sf::Color::White);
	
	// Affichage des joints
	for (auto it = mWorld.GetJointList().begin(); it != mWorld.GetJointList().end(); ++it)
	{
		(*it)->Update();
		mWindow.draw(**it);
	}

	// Affichage des objets dynamiques
	for (auto it = mWorld.GetBodyList().begin(); it != mWorld.GetBodyList().end(); ++it)
	{
		if ((*it)->GetBody()->GetType() == b2_dynamicBody)
		{
			(*it)->Update();
			mWindow.draw(**it);
		}
	}

	// Affichage des objets statiques
	for (auto it = mWorld.GetBodyList().rbegin(); it != mWorld.GetBodyList().rend(); ++it)
	{
		if ((*it)->GetBody()->GetType() == b2_staticBody)
		{
			(*it)->Update();
			mWindow.draw(**it);
		}
	}

	// Affichage du joint de la souris
	if (mMouseJoint)
	{
		mMouseJoint->Update();
		mWindow.draw(*mMouseJoint);
	}

	mWindow.display();
}

/// Appelé quand la boucle se termine
void Box2DGame::OnLoopEnd()
{
	Game::OnLoopEnd();

	// Sauvegarde la dernière position de la souris en système Box2D
	mLMp = mMp;
}

/// Appelé quand le jeu se termine
void Box2DGame::OnQuit()
{
	Game::OnQuit();

	// Supprime le joint de la souris
	if (mMouseJoint)
	{
		mWorld.DestroyJoint(mMouseJoint);
		mMouseJoint = nullptr;
	}

	// Vide les listes d'objets
	mWorld.DestroyAllJoints();
	mWorld.DestroyAllBody();
}
