#include "Box2DGame.h"
#include "LevelLoader.h"
#include "Physics/Bodies/StaticBox.h"
#include "Physics/Bodies/DynamicBox.h"
#include "Physics/Bodies/DynamicCircle.h"
#include "Physics/Joints/DistanceJoint.h"
#include "Physics/OverlappingBodyCallback.h"
#include "Physics/FirstBodyRaycastCallback.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <list>
#include <Thor/Animation.hpp>

// Ctor
Box2DGame::Box2DGame(sf::RenderWindow & window)
	: mWindow(window), mActionMap(mWindow), mResourceManager(ResourceManager::GetInstance()), mTextureMap(mResourceManager.GetTextureMap()),
	// Physique
	mGravity(0.0f, -9.8f), mWorld(mGravity)
{
	// Etats du jeu
	mPaused = false;
	mExited = false;

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
	// Les pointeurs sont aussi supprim�s dans OnQuit();

	// TODO: Ce destructeur

	delete mLevel;
	mLevel = nullptr;

	/*/ H�ro
	Ragdoll *mHeroRagdoll;
	
	// Grapin
	Body *mHookedSBody;
	b2Vec2 mHookedSAnchor;
	DistanceJoint *mHookJoint;
	sf::Clock mHookClock;

	// D�placement des objets
	MouseJoint *mMouseJoint;

	// Variables pour la cr�ation de joints
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

	// Tant que la fen�tre est ouverte
	while (mWindow.isOpen() && !mExited)
	{
		// La boucle commence
		this->OnLoopBegin();

		// Appel des �v�nements
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

/* Fonctions �v�nements */
/// Initialise le jeu
void Box2DGame::OnInit()
{
	/* Fen�trage */
	// Cr�e la vue
	mWindowView = mWindow.getDefaultView();
	mWindowView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	mRenderTextureView = mWindowView;
	
	// Initialise la renderTexture
	mRenderTexture.create(mWindow.getSize().x, mWindow.getSize().y);
	// Charge un niveau
	mLevel = new Level(&mWorld);
	LevelLoader("lvls/1.lvl", mLevel);

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

	// Cr�e le h�ro
	mHeroRagdoll = new Ragdoll(&mWorld, b2Vec3(-1.f, 2.f, 0.f));
	mHeroBody = new DynamicBox(&mWorld, b2Vec3(0.f, 200.f, 0.f), mTextureMap["hero"]);
	mHeroBody->SetType(BodyType::Bullet);
	mWorld.RegisterBody(mHeroBody);
	mHeroBody->GetBody()->SetFixedRotation(true);
	mCharacter = new Entity(&mWorld, &*(mTextureMap["hero"]), b2Vec3(0.f, 0.f, 0.f));

	/* Cr�e les actions */
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

/// Appel� quand la boucle commence
void Box2DGame::OnLoopBegin()
{
	// Sauvegarde la derni�re position de la souris
	mCurrentMousePos = i2f(sf::Mouse::getPosition());
	mCurrentMousePosRV = mRenderTexture.convertCoords(sf::Mouse::getPosition(mWindow), mRenderTextureView);
	mMp = sf2b2Vec(mCurrentMousePosRV, mWorld.GetMPP()); // syst�me Box2D
}

/// Appel� pour les �v�nements
void Box2DGame::OnEvent()
{
	/* Evenements */
	// R�cup�re les �v�nements
	mActionMap.update();

	// Invoque les callbacks
	mActionMap.invokeCallbacks(mActionCallbackSystem);

	// Gestion de la fermeture de la fen�tre
	if (mActionMap.isActive("closed"))
	{
		mExited = true;
		mWindow.close();
	}

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
		mRenderTexture.create(static_cast<unsigned int>(mWindow.getSize().x * mRenderTextureView.getViewport().width)
								, static_cast<unsigned int>(mWindow.getSize().y * mRenderTextureView.getViewport().height));
		mRenderTextureView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));
		mRenderTextureView.zoom(mCurrentZoom);
		
		mWindowView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mWindowView.setCenter(mWindowView.getSize() / 2.f); // TODO: Pourquoi ?
	}
	
	// D�placement de la vue avec les fl�ches
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

	// G�re le d�placement � la souris (clic molette)
	if (mActionMap.isActive("movingView"))
	{
		mRenderTextureView.move(- static_cast<float>(mCurrentMousePos.x - mLastMousePos.x) * mCurrentZoom,
						- static_cast<float>(mCurrentMousePos.y - mLastMousePos.y) * mCurrentZoom);
	}

	// Cr�ation d'objets
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
		mRenderTextureView.zoom(mCurrentZoom);
		mRenderTextureView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mWorld.GetPPM()));

		// Supprime les pointeurs
		mHookJoint = nullptr;
		mMouseJoint = nullptr;

		// Cr�e le h�ro
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
		// Si le grapin est d�j� accroch�, on le d�croche
		if (mHookJoint)
		{
			if (!mHookJoint->IsNull())
				mWorld.DestroyJoint(mHookJoint);
			mHookJoint = nullptr;
		}

		// Si le body est d�j� s�lectionn�, on l'accroche avec le grapin
		else if (mHookedSBody)
		{
			// Cr�e une petite AABB sur la souris
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

				// V�rifie que le body soient valides
				if (mHookedSBody->IsNull())
					mHookedSBody = nullptr;
				else
				{
					// Enregistre le body appuy�
					Body *b = (Body*) callback.GetFixture()->GetBody()->GetUserData();
					b2Vec2 v = b->GetBody()->GetLocalPoint(mMp);

					mHookJoint = new DistanceJoint(&mWorld, mHookedSBody, mHookedSAnchor, b, v);
					mWorld.RegisterJoint(mHookJoint);
					mHookClock.restart();
				}
			}
			mHookedSBody = nullptr;
		}

		// Sinon on cherche le body survoll� pour l'accrocher
		else
		{
			// Supprime le grapin existant
			if (mHookJoint)
			{
				mWorld.DestroyJoint(mHookJoint);
				mHookJoint = nullptr;
			}

			// Cr�e une petite AABB sur la souris
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
				// Enregistre le body appuy�
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
			// Cr�e une petite AABB sur la souris
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
				// V�rifie que les bodies soient valides
				if (mPinBodyA)
					if (mPinBodyA->IsNull())
						mPinBodyA = nullptr;
				if (mPinBodyB)
					if (!mPinBodyB->IsNull())
						mPinBodyB = nullptr;

				// Enregistre le body appuy�
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
			mWorld.RegisterJoint(new DistanceJoint(&mWorld, mPinBodyA, mPinAnchorA, mPinBodyB, mPinAnchorB));
			//mWorld.RegisterJoint(new RevoluteJoint(&mWorld, mPinBodyA, mPinBodyB, mPinAnchorA, true, 90.f, 270.f, true, 30.f));

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

			// R�cup�re le premier point d'intersection
			FirstBodyRaycastCallback callback;
			mWorld.RayCast(&callback, mSpliceL1, mSpliceL2);
			if (callback.mHit)
			{
				callback.mHit = false;
				mSpliceP1 = callback.mFixture->GetBody()->GetLocalPoint(callback.mPoint);

				// R�cup�re le dernier point d'intersection
				callback.SetBody(callback.mFixture->GetBody());
				mWorld.RayCast(&callback, mSpliceL2, mSpliceL1);
				if (callback.mHit)
				{
					mSpliceP2 = callback.mFixture->GetBody()->GetLocalPoint(callback.mPoint);

					// Coupe le body touch� (uniquement dynamic_box)
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

							// On a trouv� pour le point 1
							if (cop1 < 0.0002f && cop1 > -0.0002f)
							{
								p1a = v1;
								p1b = v2;
								std::cout << "trouv\x82 p1 !!" << std::endl;
							}
							std::cout << std::endl;

							// On a trouv� pour le point 2
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

							// Cr�e un shape avec ces vertices
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

	// D�placements des objets
	if (mActionMap.isActive("onMoveObject"))
	{
		// Si la souris est d�j� attach�e, on met � jour la position
		if (mMouseJoint)
		{
			mMouseJoint->SetTarget(mMp);
		}

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

	// D�placements du h�ro
	if (mHeroBody)
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

/// Appel� pour la physique
void Box2DGame::OnStepPhysics()
{
	// Simule
	mWorld.Step(1.f / 60.f, 6, 3);
	mWorld.ClearForces();

	// Destruction des bodies en dehors de la zone
	for (auto it = mWorld.GetBodyList().begin(); it != mWorld.GetBodyList().end(); )
	{
		// On supprime seulement les dynamicBodies
		if ((*it)->GetBody()->GetType() == b2_dynamicBody)
		{
			bool erase = false;
			// V�rifie si l'objet est hors du monde et pas accroch� � la souris
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

	// Mets � jour le grapin
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

	// Mets � jour le character
	mCharacter->Update();
}

bool intersect(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D)
{
	// Soit le segment [AB], et le segment [CD].
	// Je note I le vecteur AB, et J le vecteur CD
	sf::Vector2f I = B - A;
	sf::Vector2f J = D - C;

	// Si les vecteurs sont colin�aires, alors les droites sont paralleles, pas d'intersection.
	float col = I.x * J.y - I.y * J.x;
	if (col > -0.01f && col < 0.01f)
		return false;

	// Soit k le parametre du point d'intersection du segment CD sur la droite AB. on sera sur le segment si 0 < k < 1
	// Soit m le parametre du point d'intersection du segment AB sur la droite CD, on sera sur le segment si 0 < m < 1
	//
	// Soit P le point d'intersection
	//   P = A + k * I; // equation (1)
	//   P = C + m * J;
	//
	// D'o� :
	//   A + k*I = C + m*J
	//
	// On d�compose les points et vecteurs, on a :
	//   Ax + k * Ix = Cx + m * Jx
	//   Ay + k * Iy = Cy + m * Jy
	//
	//   2 �quations, 2 inconnues, en r�solvant, on trouve :

	float m = - (- I.x * A.y + I.x * C.y + I.y * A.x - I.y * C.x) / (I.x * J.y - I.y * J.x);
	float k = - (A.x * J.y - C.x * J.y - J.x * A.y + J.x * C.y) / (I.x * J.y - I.y * J.x);

	// (Notez que les d�nominateurs sont les m�mes)

	// On v�rifie que 0 < m < 1 et 0 < k < 1 --> sinon, cela veut dire que les droites s'intersectent, mais pas au niveau du segment.
	if (0.f < m && m < 1.f && 0.f < k && k < 1.f)
		return true;
	return false;
}

/// Appel� pour le rendu
void Box2DGame::OnRender()
{
	mFrameTime.restart();

	// Rendu
	mRenderTexture.clear(mLevel->GetBckgColor());
	mWindow.clear(mLevel->GetBckgColor());
	mWindow.setView(mWindowView);
	mRenderTexture.setView(mRenderTextureView);
	
	// Ombres
	float PPM = mWorld.GetPPM();
	sf::Vector2f lightPos(mCurrentMousePosRV);
	float lightRadius = 5.f * PPM;

	sf::RenderTexture shadowTex;
	shadowTex.create(mWindow.getSize().x, mWindow.getSize().y);
	shadowTex.clear(sf::Color(0, 0, 0, 0));
	shadowTex.setView(mRenderTextureView);

	sf::RectangleShape obscurity(mRenderTextureView.getSize());
	obscurity.setOrigin(obscurity.getSize() / 2.f);
	obscurity.setPosition(mRenderTextureView.getCenter());
	obscurity.setFillColor(sf::Color(0, 0, 0, 200));
	shadowTex.draw(obscurity);

	sf::RenderStates states;
	states.blendMode = sf::BlendNone;

	sf::CircleShape light;
	light.setRadius(lightRadius);
	light.setFillColor(sf::Color(0, 0, 0, 0));
	light.setOrigin(light.getGlobalBounds().width / 2.f, light.getGlobalBounds().height / 2.f);
	light.setPosition(lightPos);
	shadowTex.draw(light, states);

	// Affichage des levels de la d�co avec zindex positif
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

	// Affichage des objets dynamiques
	for (auto it = mWorld.GetBodyList().begin(); it != mWorld.GetBodyList().end(); ++it)
	{
		if ((*it)->GetBody()->GetType() == b2_dynamicBody && (*it)->IsDrawable())
		{
			(*it)->Update();
			mRenderTexture.draw(**it);

			if (abs(((b2PolygonShape*) (*it)->GetShape())->GetVertexCount()) == 4)
			{//*/
				// Garde qqs variables
				b2PolygonShape *shape = (b2PolygonShape*) (*it)->GetShape();
				unsigned int nbPts = shape->GetVertexCount();

				// R�cup�re les positions des points
				std::vector<sf::Vector2f> points(nbPts);
				for (unsigned int i = 0; i < points.size(); ++i)
				{
					points[i] = b22sfVec((*it)->GetBody()->GetWorldPoint(shape->GetVertex(i)), PPM);
				}

				// Regarde quels points sont cach�s
				// les vector<bool> sont sp�cialis�s
				std::vector<unsigned char> pointsCaches(points.size(), false);
				for (unsigned int i = 0; i < pointsCaches.size(); ++i)
				{
					// Pour chaque points => pour chaque ar�te
					for (unsigned int j = 0; j < points.size() && !pointsCaches[i]; ++j)
					{
						// Calcule le nb du prochain sommet
						unsigned int k = (j + 1 < points.size()) ? (j + 1) : 0;

						// L'ar�te n'est pas issue du sommet test�
						if (i != j && i != k)
							if (intersect(lightPos, points[i], points[j], points[k]))
								pointsCaches[i] = true;
					}
				}

				// Prend les deux points extr�mes non cach�s
				unsigned int ptE1 = 0U;
				bool b1 = false;
				// Trouve ptE1, c�d celui juste avant les cach�s
				for (unsigned int i = 0; i < points.size(); ++i)
				{
					if (!pointsCaches[i])
					{
						ptE1 = i;
						b1 = true;
					}
					else if (b1)
						break;
				}
				// Trouve ptE2, c�d celui juste apr�s les cach�s
				// Il est possible que ptE2 ne soit pas chang� ci-apr�s, mais �a veut dire qu'il vaut 0
				unsigned int ptE2 = 0U;
				bool vuCache = false;
				for (unsigned int i = 0; i < points.size(); ++i)
				{
					if (pointsCaches[i])
						vuCache = true;

					if (vuCache && !pointsCaches[i])
					{
						ptE2 = i;
						vuCache = false;
						break;
					}
				}
				
				sf::ConvexShape polygonn;
				polygonn.setPointCount(points.size());

				// Projette les points
				std::vector<sf::Vector2f> pointsProjetes(points.size());
				for (unsigned int i = 0U; i < points.size(); ++i)
				{
					pointsProjetes[i] = points[i] + (((points[i] - lightPos) / distance(points[i], lightPos)) * lightRadius * 3.f);
					polygonn.setPoint(i, pointsProjetes[i]);
				}

				// Affiche le polygone
				polygonn.setFillColor(sf::Color(0, 0, 0, 200));
				states.blendMode = sf::BlendNone;
				shadowTex.draw(polygonn, states);

				// R�cup�re les points � afficher
				std::vector<sf::Vector2f> pointsAAfficher;
				for (unsigned int i = 0U; i < points.size(); ++i)
				{
					if (!pointsCaches[i])
					{
						if (i == ptE2)
							pointsAAfficher.push_back(pointsProjetes[i]);

						pointsAAfficher.push_back(points[i]);

						if (i == ptE1)
							pointsAAfficher.push_back(pointsProjetes[i]);
					}
				}
				
				// Cr�e le polygone
				sf::ConvexShape polygon;
				polygon.setPointCount(pointsAAfficher.size());
				for (unsigned int i = 0U; i < pointsAAfficher.size(); ++i)
				{
					polygon.setPoint(i, pointsAAfficher[i]);
				}
			
				// Affiche le polygone
				polygon.setFillColor(sf::Color(0, 0, 0, 200));
				states.blendMode = sf::BlendAlpha;
				shadowTex.draw(polygon, states);
			}//*/
		}
	}
	
	// Affichage des objets statiques
	for (auto it = mWorld.GetBodyList().rbegin(); it != mWorld.GetBodyList().rend(); ++it)
	{
		if ((*it)->GetBody()->GetType() == b2_staticBody && (*it)->IsDrawable())
		{
			(*it)->Update();
			mRenderTexture.draw(**it);
			//shadowTex.draw(**it);
		}
	}

	// Affichage des objets cin�matiques
	for (auto it = mWorld.GetBodyList().rbegin(); it != mWorld.GetBodyList().rend(); ++it)
	{
		if ((*it)->GetBody()->GetType() == b2_kinematicBody && (*it)->IsDrawable())
		{
			(*it)->Update();
			mRenderTexture.draw(**it);
			//shadowTex.draw(**it);
		}
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
			std::cout << "MouseJoint supprim� � cause d'un bug (\x82""vit\x82"")." << std::endl;
			mMouseJoint = nullptr;
		}
	}
	
	// Affichage des levels de la d�co avec zindex n�gatif
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

	shadowTex.display();
	mRenderTexture.display();
	mWindow.draw(sf::Sprite(mRenderTexture.getTexture()));
	mWindow.draw(sf::Sprite(shadowTex.getTexture()));
	mWindow.display();
}

/// Appel� quand la boucle se termine
void Box2DGame::OnLoopEnd()
{
	// Sauvegarde la derni�re position de la souris en syst�me Box2D
	mLastMousePos = mCurrentMousePos;
	mLastMousePosRV = mCurrentMousePosRV;
	mLMp = mMp;
}

/// Appel� quand le jeu se termine
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
