#include "Box2DGame.h"
#include "Level/LevelLoader.h"
#include "Physics/Bodies/StaticBox.h"
#include "Physics/Bodies/DynamicBox.h"
#include "Physics/Bodies/DynamicCircle.h"
#include "Physics/Joints/DistanceJoint.h"
#include "Physics/OverlappingBodyCallback.h"
#include "Physics/FirstBodyRaycastCallback.h"
#include "Tools/utils.h"
#include <iostream>
#include <vector>
#include <list>
#include <Thor/Animation.hpp>

// Ctor
Box2DGame::Box2DGame(sf::RenderWindow & window)
	: mWindow(window), mActionMap(mWindow),
	// Ressources
	mResourceManager(ResourceManager::GetInstance()),
	mTextureMap(mResourceManager.GetTextureMap()),
	// Lumières
	mLightManager(LightManager::GetInstance()),
	mMouseLight(0.f, 0.f, 400.f, false),
	// Physique
	mGravity(0.0f, -9.8f), mWorld(mGravity)
{
	// Etats du jeu
	mPaused = false;

	mCurrentZoom = 1.f;

	mMouseJoint = nullptr;
	mPinBodyA = nullptr;
	mPinBodyB = nullptr;
	mLevel = nullptr;
	mSpliceL1 = mSpliceL2 = b2Vec2(0.f, 0.f);
	mSpliceP1 = mSpliceP2 = b2Vec2(0.f, 0.f);
	mSplice1Get = false;
	mHookJoint = nullptr;
	mHookedSBody = nullptr;
	mHeroRagdoll = nullptr;
	mHeroBody = nullptr;
}

// Dtor
Box2DGame::~Box2DGame(void)
{
	// Les pointeurs sont aussi supprimés dans OnQuit();

	// TODO: Ce destructeur

	delete mLevel;
	mLevel = nullptr;

	/*/ Héro
	Ragdoll *mHeroRagdoll;
	
	// Grapin
	Body *mHookedSBody;
	b2Vec2 mHookedSAnchor;
	DistanceJoint *mHookJoint;
	sf::Clock mHookClock;

	// Déplacement des objets
	MouseJoint *mMouseJoint;

	// Variables pour la création de joints
	Body *mPinBodyA;
	b2Vec2 mPinAnchorA; // Ancres locales aux bodies
	Body *mPinBodyB;
	b2Vec2 mPinAnchorB;*/
}

// Boucle de jeu
void Box2DGame::Run()
{
	// Appel l'initialisation
	this->OnInit();

	// Tant que la fenêtre est ouverte
	while (mWindow.isOpen())
	{
		// La boucle commence
		this->OnLoopBegin();

		// Appel des évènements
		this->OnEvent();

		// Gestion de la physique
		this->OnStepPhysics();

		// Rendu
		this->OnRender();

		// La boucle se termine
		this->OnLoopEnd();
	}

	// On quite
	this->OnQuit();
}

/* Fonctions évènements */
/// Initialise le jeu
void Box2DGame::OnInit()
{
	/* Fenêtrage */
	// Crée la vue
	mWindowView = mWindow.getDefaultView();
	mWindowView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	mRenderTextureView = mWindowView;
	
	// Initialise la renderTexture
	mRenderTexture.create(mWindow.getSize().x, mWindow.getSize().y);
	mShadowRenderTexture.create(mWindow.getSize().x, mWindow.getSize().y);

	// Charge un niveau
	mLevel = new Level(&mWorld);
	LevelLoader("lvls/1.xvl", mLevel);

	// Initialise le système de lumières
	mLightManager.Initialize(mRenderTexture, &mRenderTextureView);
	mLightManager.AddLight(&mMouseLight);

	// Centre la vue
	mCurrentZoom = mLevel->GetDefaultZoom();
	mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));
	mRenderTextureView.zoom(mLevel->GetDefaultZoom());

	// Charge les textures dans la textureKeyMap
	try {
		mTextureMap["skyrim"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/skyrim.jpg"));

		mTextureMap["hollowCircle"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hollowCircle.png"));

		mTextureMap["box"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/box.png"));
		mTextureMap["box2"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/box2.png"));
		mTextureMap["caisse"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/caisse.png"));
		mTextureMap["tonneau"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/tonneau.png"));
		mTextureMap["way"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/way.png"));
		mTextureMap["way"]->setSmooth(true);
		
		mTextureMap["ground"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/ground.png"));
		mTextureMap["lampadere"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/lampadere.png"));

		mTextureMap["ball"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/ball.png"));
		mTextureMap["circle"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/circle.png"));

		mTextureMap["hero"] = mResourceManager.acquire(thor::Resources::fromFile<sf::Texture>("tex/hero/test.png"));
	}
	catch (thor::ResourceLoadingException const& e)
	{
		std::cout << e.what() << std::endl;
		system("PAUSE");
		exit(222);
	}

	// Crée le héro
	mHeroRagdoll = new Ragdoll(&mWorld, b2Vec3(-1.f, 2.f, 0.f));
	mHeroBody = new DynamicBox(&mWorld, b2Vec3(0.f, 200.f, 0.f), mTextureMap["hero"]);
	mHeroBody->SetType(BodyType::Bullet);
	mWorld.RegisterBody(mHeroBody);
	mHeroBody->GetBody()->SetFixedRotation(true);
	mCharacter = new Entity(&mWorld, &*(mTextureMap["hero"]), b2Vec3(0.f, 0.f, 0.f));

	/* Crée les actions */
	mWindow.setKeyRepeatEnabled(false);
	mActionMap["closed"] = thor::Action(sf::Keyboard::Escape, thor::Action::ReleaseOnce) || thor::Action(sf::Event::Closed);
	//mActionCallbackSystem.connect("closed", std::bind(&sf::RenderWindow::close, &mWindow));
	mActionMap["zoomIn"] = thor::Action(sf::Keyboard::Add, thor::Action::PressOnce);
	mActionMap["zoomOut"] = thor::Action(sf::Keyboard::Subtract, thor::Action::PressOnce);
	mActionMap["zoomReset"] = thor::Action(sf::Keyboard::Numpad0, thor::Action::PressOnce);
	mActionMap["viewMoveUp"] = thor::Action(sf::Keyboard::Up, thor::Action::Hold);
	mActionMap["viewMoveDown"] = thor::Action(sf::Keyboard::Down, thor::Action::Hold);
	mActionMap["viewMoveLeft"] = thor::Action(sf::Keyboard::Left, thor::Action::Hold);
	mActionMap["viewMoveRight"] = thor::Action(sf::Keyboard::Right, thor::Action::Hold);
	mActionMap["resized"] = thor::Action(sf::Event::Resized);
	mActionMap["mouseWheelMoved"] = thor::Action(sf::Event::MouseWheelMoved);
	mActionCallbackSystem.connect("mouseWheelMoved", OnMouseWheelMoved(this));
	mActionMap["movingView"] = thor::Action(sf::Mouse::Middle, thor::Action::Hold);

	mActionMap["onLoadLevel"] = thor::Action(sf::Keyboard::R, thor::Action::ReleaseOnce);
	mActionMap["onMoveObject"] = thor::Action(sf::Mouse::Right, thor::Action::Hold);
	mActionMap["onCreateBox"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);
	mActionMap["onCreateCircle"] = thor::Action(sf::Keyboard::C, thor::Action::Hold);
	mActionMap["onCreateLamp"] = thor::Action(sf::Keyboard::L, thor::Action::ReleaseOnce);
	mActionMap["onPin"] = thor::Action(sf::Keyboard::P, thor::Action::PressOnce);
	mActionMap["onSplice"] = thor::Action(sf::Keyboard::S, thor::Action::Hold);
	mActionMap["onFire"] = thor::Action(sf::Keyboard::F, thor::Action::PressOnce);
	mActionMap["onHook"] = thor::Action(sf::Keyboard::H, thor::Action::PressOnce);
	
	mActionMap["onJump"] = thor::Action(sf::Keyboard::Up, thor::Action::PressOnce) || thor::Action(sf::Keyboard::Space, thor::Action::PressOnce);
	mActionMap["onCrawl"] = thor::Action(sf::Keyboard::Down, thor::Action::Hold);
	mActionMap["onGoLeft"] = thor::Action(sf::Keyboard::Left, thor::Action::Hold);
	mActionMap["onGoRight"] = thor::Action(sf::Keyboard::Right, thor::Action::Hold);
}

/// Appelé quand la boucle commence
void Box2DGame::OnLoopBegin()
{
	// Sauvegarde la dernière position de la souris
	mCurrentMousePos = i2f(sf::Mouse::getPosition());
	mCurrentMousePosRV = mRenderTexture.convertCoords(sf::Mouse::getPosition(mWindow), mRenderTextureView);
	mMp = sf2b2Vec(mCurrentMousePosRV, mWorld.GetMPP()); // système Box2D

	mMouseLight.SetPosition(mCurrentMousePosRV);
}

/// Appelé pour les évènements
void Box2DGame::OnEvent()
{
	// Récupère les évènements
	mActionMap.update();

	// Invoque les callbacks
	mActionMap.invokeCallbacks(mActionCallbackSystem);

	// Gère le déplacement à la souris (clic molette)
	if (mActionMap.isActive("movingView"))
	{
		mRenderTextureView.move(- static_cast<float>(mCurrentMousePos.x - mLastMousePos.x) * mCurrentZoom,
						- static_cast<float>(mCurrentMousePos.y - mLastMousePos.y) * mCurrentZoom);
	}

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
		StaticBox *l = new StaticBox(&mWorld, getVec3(mMp), mTextureMap["lampadere"], 0.1f, 0.05f);
		mWorld.RegisterBody(l);
		sf::Vector2f pos = mCurrentMousePosRV;
		pos.y -= 1.2f * mWorld.GetPPM();
		mLightManager.AddLight(new PointLight(pos, 100.2f, true, true, l));
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
	else if (mMouseJoint)
	{
		mWorld.DestroyJoint(mMouseJoint);
		mMouseJoint = nullptr;
	}

	// Déplacements du héro
	if (mHeroBody && false)
	{
		if (!mHeroBody->IsNull())
		{
			bool hasMoved = false;
			float dep = 0.f;
			float ft = mFrameTime.getElapsedTime().asSeconds();
			if (mActionMap.isActive("onJump") && !mIsJumping)
			{
				mIsJumping = true;
				mHeroBody->GetBody()->ApplyLinearImpulse(b2Vec2(0.f, 240.f * ft), mHeroBody->GetBody()->GetWorldCenter());
			}
			else if (mIsJumping)
			{
				// TODO: contacts
				if (mHeroBody->GetBody()->GetLinearVelocity().y < 0.001f && mHeroBody->GetBody()->GetLinearVelocity().y > -0.001f)
					mIsJumping = false;
			}
			if (mActionMap.isActive("onCrawl"))
			{
			}
			if (mActionMap.isActive("onGoLeft"))
			{
				dep -= 200.f * ft;
				hasMoved = true;
			}
			if (mActionMap.isActive("onGoRight"))
			{
				dep += 200.f * ft;
				hasMoved = true;
			}
			if (hasMoved)
			{
				dep = min(abs(mHeroBody->GetBody()->GetLinearVelocity().x + dep), 400.f * ft) * sign(dep);

				mHeroBody->GetBody()->ApplyForceToCenter(b2Vec2(dep, 0.f));

				if (abs(mHeroBody->GetBody()->GetLinearVelocity().x) > 500.f * ft)
					mHeroBody->GetBody()->SetLinearVelocity(b2Vec2(500.f * ft * sign(mHeroBody->GetBody()->GetLinearVelocity().x)
																	, mHeroBody->GetBody()->GetLinearVelocity().y));
			}
		}
	}

	// Déplacement de la vue avec les flèches
	if (mActionMap.isActive("viewMoveUp"))
	{
		mRenderTextureView.move(sf::Vector2f(0.f, -5.f) * mCurrentZoom);
	}
	if (mActionMap.isActive("viewMoveDown"))
	{
		mRenderTextureView.move(sf::Vector2f(0.f, 5.f) * mCurrentZoom);
	}
	if (mActionMap.isActive("viewMoveLeft"))
	{
		mRenderTextureView.move(sf::Vector2f(-5.f, 0.f) * mCurrentZoom);
	}
	if (mActionMap.isActive("viewMoveRight"))
	{
		mRenderTextureView.move(sf::Vector2f(5.f, 0.f) * mCurrentZoom);
	}

	// Charge un niveau
	if (mActionMap.isActive("onLoadLevel"))
	{
		mLightManager.DeleteLight(&mMouseLight, false);
		LevelLoader("lvls/1.xvl", mLevel);
		mLightManager.AddLight(&mMouseLight);

		// Centre la vue
		mCurrentZoom = mLevel->GetDefaultZoom();
		mRenderTextureView.zoom(mCurrentZoom);
		mRenderTextureView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));

		// Supprime les pointeurs
		mHookJoint = nullptr;
		mMouseJoint = nullptr;

		// Crée le héro
		//delete mHeroRagdoll;
		mHeroRagdoll = new Ragdoll(&mWorld, b2Vec3(-0.3f, 10.f, 0.f));
		mHeroBody = new DynamicBox(&mWorld, b2Vec3(0.f, 0.f, 0.f), mTextureMap["hero"]);
		mHeroBody->SetType(BodyType::Bullet);
		mWorld.RegisterBody(mHeroBody);
		mHeroBody->GetBody()->SetFixedRotation(true);
		//delete mCharacter;
		mCharacter = new Entity(&mWorld, &*(mTextureMap["hero"]), b2Vec3(0.f, 0.f, 0.f));
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
			if (callback.GetFixture() && mHookedSBody && callback.GetFixture()->GetBody() != mHookedSBody->GetBody())
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
					b2Vec2 v = b->GetBody()->GetLocalPoint(mMp);

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
					mPinAnchorA = mPinBodyA->GetBody()->GetLocalPoint(mMp);
				}
				else if (!mPinBodyB && mPinBodyA != (Body*) callback.GetFixture()->GetBody()->GetUserData())
				{
					mPinBodyB = (Body*) callback.GetFixture()->GetBody()->GetUserData();
					mPinAnchorB = mPinBodyB->GetBody()->GetLocalPoint(mMp);
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
			//mWorld.RegisterJoint(new RevoluteJoint(&mWorld, mPinBodyA, mPinBodyB, mPinAnchorA, true, 90.f, 270.f, true, 30.f));

			mPinBodyA = nullptr;
			mPinBodyB = nullptr;
		}
	}
	
#if 0
	// Coupe un objet // TODO: Finir
	else if (false && mActionMap.isActive("onSplice") && !mSplice1Get)
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
						b2Fixture *fix = callback.mFixture;
						b2PolygonShape *shape = (b2PolygonShape*) fix->GetShape();

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
								std::cout << "trouv\x82 pt1 !!" << std::endl;
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
#endif

	// Gestion du zoom et du resize
	if (mActionMap.isActive("zoomIn"))
	{
		mCurrentZoom *= 0.8f;
		mRenderTextureView.zoom(0.8f);
	}
    if (mActionMap.isActive("zoomOut"))
	{
		mCurrentZoom *= 1.2f;
		mRenderTextureView.zoom(1.2f);
	}
    if (mActionMap.isActive("zoomReset"))
	{
		mCurrentZoom = mLevel->GetDefaultZoom();
		mRenderTextureView.zoom(mCurrentZoom);
		mRenderTextureView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));
	}
    if (mActionMap.isActive("resized"))
	{
		mRenderTextureView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));
		mRenderTextureView.zoom(mCurrentZoom);

		mRenderTexture.create(static_cast<unsigned int>(mWindow.getSize().x * mRenderTextureView.getViewport().width)
								, static_cast<unsigned int>(mWindow.getSize().y * mRenderTextureView.getViewport().height));

		mShadowRenderTexture.create(static_cast<unsigned int>(mWindow.getSize().x * mRenderTextureView.getViewport().width)
								, static_cast<unsigned int>(mWindow.getSize().y * mRenderTextureView.getViewport().height));

		mWindowView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mWindowView.setCenter(mWindowView.getSize() / 2.f);

		mLightManager.Resize(mWindow);
	}
	
	// Gestion de la fermeture de la fenêtre
	if (mActionMap.isActive("closed"))
	{
		mWindow.close();
	}
}

/// Appelé pour la physique
void Box2DGame::OnStepPhysics()
{
	mFrameTime.restart();

	// Simule
	mWorld.Step(1.f / 60.f, 5, 3);
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
			if (mHookJoint->GetLength() - mHookClock.getElapsedTime().asSeconds() * 0.2f > 0)
				mHookJoint->SetLength(mHookJoint->GetLength() - mHookClock.getElapsedTime().asSeconds() * 8.f);
			mHookClock.restart();
		}
	}

	// Mets à jour le character
	mCharacter->Update();

	// TODO: Mettre au bon endroit -> PlayerManager / EntityManager / ... ??
	// Déplacements du héro
	if (mHeroBody && false)
	{
		if (!mHeroBody->IsNull())
		{
			bool hasMoved = false;
			float dep = 0.f;
			float ft = mFrameTime.getElapsedTime().asSeconds();
			if (mActionMap.isActive("onJump") && !mIsJumping)
			{
				mIsJumping = true;
				mHeroBody->GetBody()->ApplyLinearImpulse(b2Vec2(0.f, 240.f * ft), mHeroBody->GetBody()->GetWorldCenter());
			}
			else if (mIsJumping)
			{
				// TODO: contacts
				if (mHeroBody->GetBody()->GetLinearVelocity().y < 0.001f && mHeroBody->GetBody()->GetLinearVelocity().y > -0.001f)
					mIsJumping = false;
			}
			if (mActionMap.isActive("onCrawl"))
			{
			}
			if (mActionMap.isActive("onGoLeft"))
			{
				dep -= 200.f * ft;
				hasMoved = true;
			}
			if (mActionMap.isActive("onGoRight"))
			{
				dep += 200.f * ft;
				hasMoved = true;
			}
			if (hasMoved)
			{
				dep = min(abs(mHeroBody->GetBody()->GetLinearVelocity().x + dep), 400.f * ft) * sign(dep);

				mHeroBody->GetBody()->ApplyForceToCenter(b2Vec2(dep, 0.f));

				if (abs(mHeroBody->GetBody()->GetLinearVelocity().x) > 500.f * ft)
					mHeroBody->GetBody()->SetLinearVelocity(b2Vec2(500.f * ft * sign(mHeroBody->GetBody()->GetLinearVelocity().x)
																	, mHeroBody->GetBody()->GetLinearVelocity().y));
			}
		}
	}
}

/// Appelé pour le rendu
void Box2DGame::OnRender()
{
	if (!mLevel)
		return;

	// Rendu
	mRenderTexture.clear(mLevel->GetBckgColor());
	mWindow.clear(mLevel->GetBckgColor());
	mWindow.setView(mWindowView);
	mRenderTexture.setView(mRenderTextureView);
	mLightManager.SetView(&mRenderTextureView);
	
	// Affichage des levels de la déco avec zindex positif
	for (auto it = mLevel->GetDeco().begin(); it != mLevel->GetDeco().end(); ++it)
	{
		if (it->first > 0)
		{
			for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				mRenderTexture.draw(*it2->second);
			}
		}
	}

	// Affichage des objets physiques
	for (auto it = mWorld.GetBodyList().begin(); it != mWorld.GetBodyList().end(); ++it)
	{
			(*it)->Update();
			mRenderTexture.draw(**it);
	}
	
	// Affichage des joints
	for (auto it = mWorld.GetJointList().begin(); it != mWorld.GetJointList().end(); ++it)
	{
		(*it)->Update();
		mRenderTexture.draw(**it);
	}

	// Affichage du joint de la souris
	if (mMouseJoint)
	{
		if (!mMouseJoint->IsNull())
		{
			mMouseJoint->Update();
			mRenderTexture.draw(*mMouseJoint);
		}
		else
		{
			std::cout << "MouseJoint supprimé à cause d'un bug (\x82""vit\x82"")." << std::endl;
			mMouseJoint = nullptr;
		}
	}
	
	// Affichage des levels de la déco avec zindex négatif
	for (auto it = mLevel->GetDeco().begin(); it != mLevel->GetDeco().end() && it->first < 0; ++it)
	{
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			mRenderTexture.draw(*it2->second);
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

	// Finalise le rendu des objets
	mRenderTexture.display();
	mWindow.draw(sf::Sprite(mRenderTexture.getTexture()));

	// Création des ombres
	if (mLevel->GetLightning())
	{
		mLightManager.Update();
		mWindow.draw(mLightManager);
	}

	// Affichage du tout
	mWindow.display();
}

/// Appelé quand la boucle se termine
void Box2DGame::OnLoopEnd()
{
	// Sauvegarde la dernière position de la souris en système Box2D
	mLastMousePos = mCurrentMousePos;
	mLastMousePosRV = mCurrentMousePosRV;
	mLMp = mMp;
}

/// Appelé quand le jeu se termine
void Box2DGame::OnQuit()
{
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

/* Pour la molette de la souris */
OnMouseWheelMoved::OnMouseWheelMoved(Box2DGame *_game)
: mGame(_game)
{
}
void OnMouseWheelMoved::operator() (thor::ActionContext<std::string> context)
{
	if (context.event->mouseWheel.delta > 0)
	{
		mGame->mCurrentZoom *= 0.8f;
		mGame->mRenderTextureView.zoom(0.8f);
		mGame->mWindow.setView(mGame->mRenderTextureView);
	}
	else
	{
		mGame->mCurrentZoom *= 1.2f;
		mGame->mRenderTextureView.zoom(1.2f);
		mGame->mWindow.setView(mGame->mRenderTextureView);
	}
}
