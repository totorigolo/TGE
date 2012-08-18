#include "Box2DGame.h"
#include "config.h"
#include "utils.h"
#include "OverlappingBodyCallback.h"
#include "StaticBox.h"
#include "DynamicBox.h"
#include "DynamicCircle.h"
#include <iostream>

// Ctor
Box2DGame::Box2DGame(sf::RenderWindow & window)
	: Game(window),
	// Physique
	mGravity(0.0f, -9.8f), mWorld(mGravity)
{
	mMouseJoint = nullptr;
}

// Dtor
Box2DGame::~Box2DGame(void)
{
}

/* Surcharge des fonctions �v�nements */
/// Initialise le jeu
void Box2DGame::OnInit()
{
	Game::OnInit();

	// Centre la vue
	mView.setCenter(sf::Vector2f(0.f, 0.f));
	mWindow.setView(mView);

	// Charge les textures dans la textureKeyMap
	try {
		mTextureMap["box"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("box.png"));
		mTextureMap["box2"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("box2.png"));
		mTextureMap["ground"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("ground.png"));
		mTextureMap["circle"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("circle.png"));
	}
	catch (thor::ResourceLoadingException const& e)
	{
		std::cout << e.what() << std::endl;
		system("PAUSE");
		exit(222);
	}

	// Cr�e le sol
	mStaticBodyList.push_back(new StaticBox(&mWorld, b2Vec2(0.f, -5.f), mTextureMap["ground"]));

	/* Cr�e les actions */
	mActionMap["onMoveObject"] = thor::Action(sf::Mouse::Right, thor::Action::Hold);
	mActionMap["onUnMoveObject"] = thor::Action(sf::Mouse::Right, thor::Action::ReleaseOnce);
	mActionMap["onCreateBox"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);
	mActionMap["onCreateCircle"] = thor::Action(sf::Keyboard::C, thor::Action::Hold);
}

/// Appel� quand la boucle commence
void Box2DGame::OnLoopBegin()
{
	Game::OnLoopBegin();

	// Converti la position de la souris en syst�me Box2D
	mMp.x = mCurrentMousePosRV.x * MPP;
	mMp.y = - mCurrentMousePosRV.y * MPP;

	// Simule
	mWorld.Step(1.f / 60.f, 8, 3);
}

/// Appel� pour les �v�nements
void Box2DGame::OnEvent()
{
	Game::OnEvent();

	// Cr�ation d'objets
	if (mActionMap.isActive("onCreateBox"))
	{
		std::string s = ((randMinMax(0, 1) == 0) ? "box" : "box2");
		mBodyList.push_back(new DynamicBox(&mWorld, mMp, mTextureMap[s]));
	}
	if (mActionMap.isActive("onCreateCircle"))
	{
		mBodyList.push_back(new DynamicCircle(&mWorld, mMp, mTextureMap["circle"]));
	}

	// D�placements des objets
	if (mActionMap.isActive("onUnMoveObject"))
	{
		if (mMouseJoint)
			mWorld.DestroyJoint(mMouseJoint);
		mMouseJoint = nullptr;
	}
	if (mActionMap.isActive("onMoveObject"))
	{
		// Si la souris est d�j� attach�e, on met � jour la position
		if (mMouseJoint)
			mMouseJoint->SetTarget(mMp);

		// Sinon on recherche l'objet sous la souris et on l'attache
		else
		{
			// Cr�e une petite AABB sur la souris
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
				b2Body* body = callback.GetFixture()->GetBody();
				b2MouseJointDef md;
				md.bodyA = body;
				md.bodyB = body;
				md.target = mMp;
				md.maxForce = 80000.0f * body->GetMass();
				mMouseJoint = (b2MouseJoint*)mWorld.CreateJoint(&md);
				body->SetAwake(true);
			}
		}
	}
}

/// Appel� pour le rendu
void Box2DGame::OnRender()
{
	mWindow.clear(sf::Color::White);
	
	// Affichage des objets dynamiques
	for (auto it = mBodyList.begin(); it != mBodyList.end(); )
	{
		bool erase = false;
		// V�rifie si l'objet n'est pas statique
		if (!(*it)->IsStatic())
		{
			// V�rifie si l'objet est hors du monde et pas accroch� � la souris
			if (!(*it)->IsInRange(b2Vec2(20.f, 5.f), b2Vec2(-00.f, -30.f)))
			{
				erase = true;
				if (mMouseJoint)
					if ((*it)->GetBody() == mMouseJoint->GetBodyA())
						erase = false;
			}
		}

		// Supprime le body
		if (erase)
		{
			(*it)->Destroy();
			it = mBodyList.erase(it);
		}

		// Sinon on l'affiche
		else
		{
			(*it)->Update();
			mWindow.draw(**it);
			++it;
		}
	}

	// Affichage des objets statiques
	for (auto it = mStaticBodyList.rbegin(); it != mStaticBodyList.rend(); ++it)
	{
		(*it)->Update();
		mWindow.draw(**it);
	}

	mWindow.display();
}

/// Appel� quand la boucle se termine
void Box2DGame::OnLoopEnd()
{
	Game::OnLoopEnd();

	// Sauvegarde la derni�re position de la souris en syst�me Box2D
	mLMp = mMp;
}

/// Appel� quand le jeu se termine
void Box2DGame::OnQuit()
{
	Game::OnQuit();
}
