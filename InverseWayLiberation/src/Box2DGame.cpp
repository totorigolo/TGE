#include "Box2DGame.h"
#include "LevelLoader.h"
#include "Physics/StaticBox.h"
#include "Physics/DynamicBox.h"
#include "Physics/DynamicCircle.h"
#include "Physics/DistanceJoint.h"
#include "Physics/OverlappingBodyCallback.h"
#include "Physics/FirstBodyRaycastCallback.h"
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
	mLevel = nullptr;
	mSpliceL1 = mSpliceL2 = b2Vec2(0.f, 0.f);
	mSpliceP1 = mSpliceP2 = b2Vec2(0.f, 0.f);
	mSplice1Get = false;
	mHookJoint = nullptr;
	mHookedSBody = nullptr;
}

// Dtor
Box2DGame::~Box2DGame(void)
{
	// Les pointeurs sont supprimés dans OnQuit();
}

/* Surcharge des fonctions évènements */
/// Initialise le jeu
void Box2DGame::OnInit()
{
	Game::OnInit();

	// Charge un niveau
	mLevel = new Level(&mWorld, &mTextureCache, &mTextureMap);
	LevelLoader("lvls/1.lvl", mLevel);

	// Centre la vue
	mCurrentZoom = mLevel->GetDefaultZoom();
	mView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));
	mView.zoom(mLevel->GetDefaultZoom());
	mWindow.setView(mView);

	// Charge les textures dans la textureKeyMap
	try {
		mTextureMap["hollowCircle"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/hollowCircle.png"));

		mTextureMap["box"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/box.png"));
		mTextureMap["box2"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/box2.png"));
		mTextureMap["caisse"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/caisse.png"));
		mTextureMap["tonneau"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/tonneau.png"));
		mTextureMap["way"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/way.png"));
		
		mTextureMap["ground"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/ground.png"));
		mTextureMap["lampadere"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/lampadere.png"));

		mTextureMap["ball"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/ball.png"));
		mTextureMap["circle"] = mTextureCache.acquire(thor::Resources::fromFile<sf::Texture>("tex/circle.png"));
	}
	catch (thor::ResourceLoadingException const& e)
	{
		std::cout << e.what() << std::endl;
		system("PAUSE");
		exit(222);
	}

	// Crée le sol
	//mWorld.RegisterBody(new StaticBox(&mWorld, b2Vec3(0.f, -6.f, 0.f), mTextureMap["ground"], 0.4f));

	/* Crée les actions */
	mActionMap["onLoadLevel"] = thor::Action(sf::Keyboard::R, thor::Action::ReleaseOnce);
	mActionMap["onMoveObject"] = thor::Action(sf::Mouse::Right, thor::Action::Hold);
	mActionMap["onCreateBox"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);
	mActionMap["onCreateCircle"] = thor::Action(sf::Keyboard::C, thor::Action::Hold);
	mActionMap["onCreateLamp"] = thor::Action(sf::Keyboard::L, thor::Action::ReleaseOnce);
	mActionMap["onPin"] = thor::Action(sf::Keyboard::P, thor::Action::PressOnce);
	mActionMap["onSplice"] = thor::Action(sf::Keyboard::S, thor::Action::Hold);
	mActionMap["onFire"] = thor::Action(sf::Keyboard::F, thor::Action::PressOnce);
	mActionMap["onHook"] = thor::Action(sf::Keyboard::H, thor::Action::PressOnce);
}

/// Appelé quand la boucle commence
void Box2DGame::OnLoopBegin()
{
	Game::OnLoopBegin();

	// Converti la position de la souris en système Box2D
	mMp = sf2b2Vec(mCurrentMousePosRV, mWorld.GetMPP());

	// Simule
	mWorld.Step(1.f / 60.f, 8, 3);
	mWorld.ClearForces();

	// Destruction des bodies en dehors de la zone
	for (auto it = mWorld.GetBodyList().begin(); it != mWorld.GetBodyList().end(); )
	{
		// On supprime seulement les dynamicBodies
		if ((*it)->GetBody()->GetType() == b2_dynamicBody)
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
		else
			++it;
	}

	// Mets à jour le grapin
	if (mHookJoint)
	{
		if (mHookJoint->IsNull() || !mHookJoint->GetBodyA() || !mHookJoint->GetBodyB())
			mHookJoint = nullptr;
		else
		{
			if (mHookJoint->GetLenght() - mHookClock.getElapsedTime().asSeconds() * 0.2f > 0)
				mHookJoint->SetLenght(mHookJoint->GetLenght() - mHookClock.getElapsedTime().asSeconds() * 8.f);
			mHookClock.restart();
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
		std::string list[] = {"box", "box2", "caisse", "way", "tonneau"};
		mWorld.RegisterBody(new DynamicBox(&mWorld, getVec3(mMp), mTextureMap[randomElement(list, 5)]));
	}
	if (mActionMap.isActive("onCreateCircle"))
	{
		std::string list[] = {"ball", "circle"};
		mWorld.RegisterBody(new DynamicCircle(&mWorld, getVec3(mMp), mTextureMap[randomElement(list, 2)], 1.f, 0.2f, 0.5f));
	}
	if (mActionMap.isActive("onCreateLamp"))
	{
		mWorld.RegisterBody(new StaticBox(&mWorld, getVec3(mMp), mTextureMap["lampadere"], 0.1f, 0.05f));
	}
	
	// Charge un niveau
	if (mActionMap.isActive("onLoadLevel"))
	{
		LevelLoader("lvls/1.lvl", mLevel);

		// Centre la vue
		mCurrentZoom = mLevel->GetDefaultZoom();
		mView.setSize(mWindow.getSize().x * mView.getViewport().width, mWindow.getSize().y * mView.getViewport().height);
		mView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));
		mView.zoom(mLevel->GetDefaultZoom());
		mWindow.setView(mView);

		// Supprime les pointeurs
		mHookJoint = nullptr;
		mMouseJoint = nullptr;
	}

	// "Surcharge" des zooms
    if (mActionMap.isActive("zoomReset"))
	{
		mCurrentZoom = mLevel->GetDefaultZoom();
		mView.setSize(mWindow.getSize().x * mView.getViewport().width, mWindow.getSize().y * mView.getViewport().height);
		mView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));
		mView.zoom(mLevel->GetDefaultZoom());
		mWindow.setView(mView);
	}
    if (mActionMap.isActive("resized"))
	{
		mView.setSize(mWindow.getSize().x * mView.getViewport().width, mWindow.getSize().y * mView.getViewport().height);
		mView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));
		mView.zoom(mCurrentZoom);
		mWindow.setView(mView);
	}
	
	// Grapin
	if (mActionMap.isActive("onHook"))
	{
		// Si le grapin est déjà accroché, on le décroche
		if (mHookJoint)
		{
			if (!mHookJoint->IsNull())
				mWorld.DestroyJoint(mHookJoint);
			mHookJoint = nullptr;
		}

		// Si le body est déjà sélectionné, on l'accroche avec le grapin
		else if (mHookedSBody)
		{
			// Crée une petite AABB sur la souris
			b2AABB aabb;
			b2Vec2 d;
			d.Set(0.001f, 0.001f);
			aabb.lowerBound = mMp - d;
			aabb.upperBound = mMp + d;

			// Demande au monde les formes qui sont sous l'AABB
			OverlappingBodyCallback callback(mMp, false);
			mWorld.QueryAABB(&callback, aabb);

			// Il y a un objet, on le retient
			if (callback.GetFixture() && mHookedSBody)
			{
				if (mHookJoint)
					mWorld.DestroyJoint(mHookJoint);

				// Vérifie que le body soient valides
				if (mHookedSBody->IsNull())
					mHookedSBody = nullptr;
				else
				{
					// Enregistre le body appuyé
					Body *b = (Body*) callback.GetFixture()->GetBody()->GetUserData();
					b2Vec2 v = b2MulT(b2Rot(b->GetBody()->GetAngle()), mMp - b->GetBody()->GetPosition());

					mHookJoint = new DistanceJoint(&mWorld, mHookedSBody, mHookedSAnchor, b, v);
					mWorld.RegisterJoint(mHookJoint);
					mHookClock.restart();
				}
			}
			mHookedSBody = nullptr;
		}

		// Sinon on cherche le body survollé pour l'accrocher
		else
		{
			// Supprime le grapin existant
			if (mHookJoint)
			{
				mWorld.DestroyJoint(mHookJoint);
				mHookJoint = nullptr;
			}

			// Crée une petite AABB sur la souris
			b2AABB aabb;
			b2Vec2 d;
			d.Set(0.001f, 0.001f);
			aabb.lowerBound = mMp - d;
			aabb.upperBound = mMp + d;

			// Demande au monde les formes qui sont sous l'AABB
			OverlappingBodyCallback callback(mMp, false);
			mWorld.QueryAABB(&callback, aabb);

			// Il y a un objet, on le retient
			if (callback.GetFixture())
			{
				// Enregistre le body appuyé
				mHookedSBody = (Body*) callback.GetFixture()->GetBody()->GetUserData();
				mHookedSAnchor = b2MulT(b2Rot(mHookedSBody->GetBody()->GetAngle()), mMp - mHookedSBody->GetBody()->GetPosition());
			}
		}
	}

	// Epingle un objet
	if (mActionMap.isActive("onPin"))
	{
		// Enregistre les bobies et leurs ancres
		if (!mPinBodyA || !mPinBodyB)
		{
			// Crée une petite AABB sur la souris
			b2AABB aabb;
			b2Vec2 d;
			d.Set(0.001f, 0.001f);
			aabb.lowerBound = mMp - d;
			aabb.upperBound = mMp + d;

			// Demande au monde les formes qui sont sous l'AABB
			OverlappingBodyCallback callback(mMp, false);
			mWorld.QueryAABB(&callback, aabb);

			// Il y a un objet, on le retient
			if (callback.GetFixture())
			{
				// Vérifie que les bodies soient valides
				if (mPinBodyA)
					if (mPinBodyA->IsNull())
						mPinBodyA = nullptr;
				if (mPinBodyB)
					if (!mPinBodyB->IsNull())
						mPinBodyB = nullptr;

				// Enregistre le body appuyé
				if (!mPinBodyA && mPinBodyB != (Body*) callback.GetFixture()->GetBody()->GetUserData())
				{
					mPinBodyA = (Body*) callback.GetFixture()->GetBody()->GetUserData();
					mPinAnchorA = b2MulT(b2Rot(mPinBodyA->GetBody()->GetAngle()), mMp - mPinBodyA->GetBody()->GetPosition());
				}
				else if (!mPinBodyB && mPinBodyA != (Body*) callback.GetFixture()->GetBody()->GetUserData())
				{
					mPinBodyB = (Body*) callback.GetFixture()->GetBody()->GetUserData();
					mPinAnchorB = b2MulT(b2Rot(mPinBodyB->GetBody()->GetAngle()), mMp - mPinBodyB->GetBody()->GetPosition());
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
			mWorld.RegisterJoint(new DistanceJoint(&mWorld, mPinBodyA, mPinAnchorA, mPinBodyB, mPinAnchorB));

			mPinBodyA = nullptr;
			mPinBodyB = nullptr;
		}
	}

	// Coupe un objet // TODO: Finir
	if (false && mActionMap.isActive("onSplice") && !mSplice1Get)
	{
		mSpliceL1 = mMp;
		mSplice1Get = true;
	}
	else if (false && mSplice1Get)
	{
		mSpliceL2 = mMp;

		if (!(mSpliceL1 == mSpliceL2) && !mActionMap.isActive("onSplice"))
		{
			mSplice1Get = false;

			// Récupère le premier point d'intersection
			FirstBodyRaycastCallback callback;
			mWorld.RayCast(&callback, mSpliceL1, mSpliceL2);
			if (callback.mHit)
			{
				callback.mHit = false;
				mSpliceP1 = callback.mFixture->GetBody()->GetLocalPoint(callback.mPoint);

				// Récupère le dernier point d'intersection
				callback.SetBody(callback.mFixture->GetBody());
				mWorld.RayCast(&callback, mSpliceL2, mSpliceL1);
				if (callback.mHit)
				{
					mSpliceP2 = callback.mFixture->GetBody()->GetLocalPoint(callback.mPoint);

					// Coupe le body touché (uniquement dynamic_box)
					if (callback.mFixture->GetBody()->GetType() == b2_dynamicBody && callback.mFixture->GetType() == b2Shape::e_polygon)
					{
						b2Body *body = callback.mFixture->GetBody();
						b2Fixture *fix = callback.mFixture;
						b2PolygonShape *shape = (b2PolygonShape*) fix->GetShape();
						int count = shape->GetVertexCount();

						int p1a = -1, p1b = -1;
						int p2a = -1, p2b = -1;

						// Regarde entre quels sommets sont les points
						for (int i = 0; i < shape->GetVertexCount(); ++i)
						{
							int v1 = (i == shape->GetVertexCount()) ? 0 : i;
							int v2 = (v1 + 1 == shape->GetVertexCount()) ? 0 : v1 + 1;

							std::cout << "Vertices " << v1 << " et " << v2 << std::endl;

							b2Vec2 u = shape->GetVertex(v2) - shape->GetVertex(v1);
							b2Vec2 v = mSpliceP1 - shape->GetVertex(v1);
							b2Vec2 w = mSpliceP2 - shape->GetVertex(v1);

							std::cout << "u (" << u.x << "," << u.y << ") et " << "v (" << v.x << "," << v.y << ")" << std::endl;

							float cop1 = (u.x * v.y) - (v.x * u.y);
							float cop2 = (u.x * w.y) - (w.x * u.y);
							std::cout << cop1 << std::endl;
							std::cout << cop2 << std::endl;

							// On a trouvé pour le point 1
							if (cop1 < 0.0002f && cop1 > -0.0002f)
							{
								p1a = v1;
								p1b = v2;
								std::cout << "trouv\x82 p1 !!" << std::endl;
							}
							std::cout << std::endl;

							// On a trouvé pour le point 2
							if (cop2 < 0.0002f && cop2 > -0.0002f)
							{
								p2a = v1;
								p2b = v2;
								std::cout << "trouv\x82 p2 !!" << std::endl;
							}
							std::cout << std::endl;
						}
						std::cout << std::endl << std::endl;

						if (p1a != -1 && p1b != -1 && p2a != -1 && p2b != -1)
						{
							// On ajoute les points dans la liste des vertices
							int count = shape->GetVertexCount() + 2;
							b2Vec2 vertices[8];
							vertices[0];
							int offset = 0;
							for (int i = 0; i < count; ++i)
							{
								if (p1a + 1 == i)
								{
									vertices[i] = mSpliceP1;
									offset += 1;
								}
								else if (p2a + 1 == i)
								{
									vertices[i] = mSpliceP2;
									offset += 1;
								}
								else
								{
									vertices[i] = shape->GetVertex((i - offset < 0) ? shape->GetVertexCount() - offset : i - offset);
								}
							}
							
							for (int i = 0; i < shape->GetVertexCount(); ++i)
							{
								std::cout << shape->GetVertex(i).x << "," << shape->GetVertex(i).y << std::endl;
							}
							std::cout << "----" << std::endl;
							for (int i = 0; i < count; ++i)
							{
								std::cout << vertices[i].x << "," << vertices[i].y << std::endl;
							}

							// Crée un shape avec ces vertices
							b2PolygonShape *newShape = new b2PolygonShape();
							newShape->Set(vertices, count);
							
							b2FixtureDef fixtureDef;
							fixtureDef.density = callback.mFixture->GetDensity();
							fixtureDef.friction = callback.mFixture->GetFriction();
							fixtureDef.restitution = callback.mFixture->GetRestitution();
							fixtureDef.shape = newShape;

							//body->DestroyFixture(callback.mFixture);
							//body->CreateFixture(&fixtureDef);
						}
					}
				}
			}
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
				mMouseJoint = new MouseJoint(&mWorld, body, mWorld.GetAnyStaticBody(), mMp, 999999999.f * b2body->GetMass());
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
	mWindow.clear(mLevel->GetBckgColor());
	
	// Affichage des levels de la déco avec zindex positif
	for (auto it = mLevel->GetDeco().begin(); it != mLevel->GetDeco().end(); ++it)
	{
		if (it->first > 0)
		{
			for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				mWindow.draw(*it2->second);
			}
		}
	}

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
	
	// Affichage des levels de la déco avec zindex négatif
	for (auto it = mLevel->GetDeco().begin(); it != mLevel->GetDeco().end() && it->first < 0; ++it)
	{
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			mWindow.draw(*it2->second);
		}
	}

	// Affichage du laser-spliter
	if (false)
	{
		sf::VertexArray splicer(sf::LinesStrip, 2);
		splicer[0].position = b22sfVec(mSpliceL1, mWorld.GetPPM());
		splicer[1].position = b22sfVec(mSpliceL2, mWorld.GetPPM());
		splicer[0].color = sf::Color::Cyan;
		splicer[1].color = sf::Color::Cyan;
		mWindow.draw(splicer);

		sf::VertexArray laser(sf::LinesStrip, 2);
		laser[0].position = b22sfVec(mSpliceP1, mWorld.GetPPM());
		laser[1].position = b22sfVec(mSpliceP2, mWorld.GetPPM());
		laser[0].color = sf::Color::Red;
		laser[1].color = sf::Color::Red;
		mWindow.draw(laser);

		sf::Sprite s(*mTextureMap["hollowCircle"]);
		s.setOrigin(u2f(s.getTexture()->getSize()) / 2.f);
		s.setPosition(b22sfVec(mSpliceP1, mWorld.GetPPM()));
		mWindow.draw(s);
		s.setPosition(b22sfVec(mSpliceP2, mWorld.GetPPM()));
		mWindow.draw(s);
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
