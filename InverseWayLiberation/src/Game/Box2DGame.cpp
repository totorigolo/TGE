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
	: mWindow(window), mQuit(false), mActionMap(mWindow),
	// Ressources
	mResourceManager(ResourceManager::GetInstance()),
	mTextureMap(mResourceManager.GetTextureMap()),
	// Lumières
	//mLightManager(LightManager::GetInstance()),
	//mMouseLight(0.f, 0.f, 400.f, false),
	// Physique
	mGravity(0.0f, -9.8f), mPhysicMgr(mGravity)
{
	// Etats du jeu
	mPaused = false;

	mCurrentZoom = 1.f;

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
	mCurrentZoom = 1.f;

	// Initialise le monde
	mPhysicMgr.SetTimeStep(1.f / 60.f);
	mPhysicMgr.SetDebugDrawTarget(&mRenderTexture);

	// Charge un niveau
	mLevel = new Level(&mPhysicMgr);
	LevelLoader("lvls/1.xvl", mLevel);
	if (!mLevel->IsCharged())
		return false;

	// Charge les scripts
	mScriptManager.LoadScriptFromFile("test");

	/* Fenêtrage */
	// Crée la vue
	mWindowView = mWindow.getDefaultView();
	mWindowView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	mRenderTextureView = mWindowView;
	
	// Initialise la renderTexture
	mRenderTexture.create(mWindow.getSize().x, mWindow.getSize().y);
	mShadowRenderTexture.create(mWindow.getSize().x, mWindow.getSize().y);

	// Initialise le système de lumières
	//mLightManager.Initialize(mRenderTexture, &mRenderTextureView);
	//mLightManager.AddLight(&mMouseLight);

	// Centre la vue
	mCurrentZoom = mLevel->GetDefaultZoom();
	mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mPhysicMgr.GetPPM()));
	mRenderTextureView.zoom(mLevel->GetDefaultZoom());

	// Initialise le système d'actions
	mActionMap.clearActions();
	mActionCallbackSystem.clearAllConnections();

	// Crée les actions
	// TODO: Callbacks et InputManager
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
	mActionMap["onCreateRagdoll"] = thor::Action(sf::Keyboard::T, thor::Action::ReleaseOnce);
	mActionMap["onCreateLamp"] = thor::Action(sf::Keyboard::L, thor::Action::ReleaseOnce);
	mActionMap["onPin"] = thor::Action(sf::Keyboard::P, thor::Action::PressOnce);
	mActionMap["onSplice"] = thor::Action(sf::Keyboard::S, thor::Action::Hold);
	mActionMap["onFire"] = thor::Action(sf::Keyboard::F, thor::Action::PressOnce);
	mActionMap["onHook"] = thor::Action(sf::Keyboard::H, thor::Action::PressOnce);
	
	mActionMap["onJump"] = thor::Action(sf::Keyboard::Z, thor::Action::PressOnce) || thor::Action(sf::Keyboard::Space, thor::Action::PressOnce);
	mActionMap["onCrawl"] = thor::Action(sf::Keyboard::S, thor::Action::PressOnce);
	mActionMap["onGoLeft"] = thor::Action(sf::Keyboard::Q, thor::Action::Hold);
	mActionMap["onGoRight"] = thor::Action(sf::Keyboard::D, thor::Action::Hold);

	mActionMap["onScriptHello"] = thor::Action(sf::Keyboard::K, thor::Action::ReleaseOnce);

	return true;
}

/// Appelé quand la boucle commence
void Box2DGame::OnLoopBegin()
{
	// Sauvegarde la dernière position de la souris
	mCurrentMousePos = i2f(sf::Mouse::getPosition());
	mCurrentMousePosRV = mRenderTexture.convertCoords(sf::Mouse::getPosition(mWindow), mRenderTextureView);
	mMp = sf2b2Vec(mCurrentMousePosRV, mPhysicMgr.GetMPP()); // système Box2D

	//mMouseLight.SetPosition(mCurrentMousePosRV);
}

/// Appelé pour les évènements
void Box2DGame::OnEvent()
{
	// Récupère les évènements
	mActionMap.update();

	// Invoque les callbacks
	mActionMap.invokeCallbacks(mActionCallbackSystem);

	// Déplacements du joueur
	if (mActionMap.isActive("onJump"))
	{
		if (mLevel->GetPlayer())
			mLevel->GetPlayer()->GetEvent(Player::Jump);
	}
	if (mActionMap.isActive("onGoLeft"))
	{
		if (mLevel->GetPlayer())
			mLevel->GetPlayer()->GetEvent(Player::Left);
	}
	if (mActionMap.isActive("onGoRight"))
	{
		if (mLevel->GetPlayer())
			mLevel->GetPlayer()->GetEvent(Player::Right);
	}
	if (mActionMap.isActive("onCrawl"))
	{
		if (mLevel->GetPlayer())
			mLevel->GetPlayer()->GetEvent(Player::Crounch);
	}

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
		EntityFactory::CreateBox(&mPhysicMgr, getVec3(mMp), list, 5);
	}
	if (mActionMap.isActive("onCreateCircle"))
	{
		std::string list[] = {"ball", "circle"};
		EntityFactory::CreateCircle(&mPhysicMgr, getVec3(mMp), list, 2);
	}
	if (mActionMap.isActive("onCreateRagdoll"))
	{
		EntityFactory::CreateRagdoll(&mPhysicMgr, mMp);
	}
	if (mActionMap.isActive("onCreateLamp"))
	{
		EntityFactory::CreateLamp(&mPhysicMgr, getVec3(mMp), -1);
	}
	
	// Déplacements des objets
	if (mActionMap.isActive("onMoveObject"))
	{
		// Si la souris est déjà attachée, on met à jour la position
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
		// Supprime les pointeurs
		mGrapnel->Destroy();
		if (mPhysicMgr.JointExists(mMouseJointID))
			mPhysicMgr.DestroyJoint(mMouseJointID);
		mMouseJointID = -1;
		mHookedSBody = nullptr;
		mPinBodyA = nullptr;
		mPinBodyB = nullptr;

		//mLightManager.DeleteLight(&mMouseLight, false);
		LevelLoader("lvls/1.xvl", mLevel);
		//mLightManager.AddLight(&mMouseLight);

		// Centre la vue
		mCurrentZoom = mLevel->GetDefaultZoom();
		mRenderTextureView.zoom(mCurrentZoom);
		mRenderTextureView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mPhysicMgr.GetPPM()));
	}

	// Grappin
	if (mActionMap.isActive("onHook"))
	{
		// Si le grapin n'existe pas, on le crée
		if (!mGrapnel)
		{
			mGrapnel = new Grapnel(&mPhysicMgr, -1);
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
				//mHookedSAnchor = b2MulT(b2Rot(mHookedSBody->GetAngle()), mMp - mHookedSBody->GetPosition());
			}
		}
	}

	// Epingle un objet
	if (mActionMap.isActive("onPin"))
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
			new DistanceJoint(&mPhysicMgr, mPinBodyA, mPinAnchorA, mPinBodyB, mPinAnchorB);

			mPinBodyA = nullptr;
			mPinBodyB = nullptr;
		}
	}
	
#if 0
	// TODO: Finir
	// Coupe un objet
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
			mPhysicMgr.RayCast(&callback, mSpliceL1, mSpliceL2);
			if (callback.mHit)
			{
				callback.mHit = false;
				mSpliceP1 = callback.mFixture->GetBody()->GetLocalPoint(callback.mPoint);

				// Récupère le dernier point d'intersection
				callback.SetBody(callback.mFixture->GetBody());
				mPhysicMgr.RayCast(&callback, mSpliceL2, mSpliceL1);
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
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mPhysicMgr.GetPPM()));
	}
    if (mActionMap.isActive("resized"))
	{
		mRenderTextureView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mRenderTextureView.setCenter(b22sfVec(mLevel->GetOriginView(), mPhysicMgr.GetPPM()));
		mRenderTextureView.zoom(mCurrentZoom);

		mRenderTexture.create(static_cast<unsigned int>(mWindow.getSize().x * mRenderTextureView.getViewport().width)
								, static_cast<unsigned int>(mWindow.getSize().y * mRenderTextureView.getViewport().height));

		mShadowRenderTexture.create(static_cast<unsigned int>(mWindow.getSize().x * mRenderTextureView.getViewport().width)
								, static_cast<unsigned int>(mWindow.getSize().y * mRenderTextureView.getViewport().height));

		mWindowView.setSize(u2f(mWindow.getSize()) * sf::Vector2f(mRenderTextureView.getViewport().width, mRenderTextureView.getViewport().height));
		mWindowView.setCenter(mWindowView.getSize() / 2.f);

		//mLightManager.Resize(mWindow);
	}
	
	// Gestion de la fermeture de la fenêtre
	if (mActionMap.isActive("closed"))
	{
		mQuit = true;
	}

	// Scripts
	if (mActionMap.isActive("onScriptHello"))
	{
		mScriptManager.ExecuteScript("test", "void hello()");
	}
}

/// Appelé pour la logique
void Box2DGame::OnLogic()
{
}

/// Appelé pour la physique
void Box2DGame::OnStepPhysics()
{
	mFrameTime.restart();

	// Simule
	mPhysicMgr.Step(7, 4);
	mPhysicMgr.GetWorld()->ClearForces();

	// Destruction des bodies en dehors de la zone
	b2Body *b = mPhysicMgr.GetBodyList(), *bb = nullptr;
	while (b)
	{
		// On supprime seulement les dynamicBodies
		if (b->GetType() == b2_dynamicBody)
		{
			bool erase = false;

			// Vérifie si l'objet est hors du monde
			b2Vec2 pos = b->GetPosition();
			if (pos.x < -200.f || pos.x > 200.f || pos.y < -200.f || pos.y > 1000.f)
			{
				erase = true;

				// Vérifie si l'objet n'est pas accroché à la souris
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

/// Appelé pour les mises à jour
void Box2DGame::OnUpdate()
{
	// Met à jour les Joints
	mPhysicMgr.UpdateJoints();

	// Met à jour le niveau
	mLevel->Update();
}

/// Appelé pour le rendu
void Box2DGame::OnRender()
{
	assert(mLevel && "est invalide.");

	// Rendu
	mRenderTexture.clear(mLevel->GetBckgColor());
	mWindow.clear(mLevel->GetBckgColor());
	mWindow.setView(mWindowView);
	mRenderTexture.setView(mRenderTextureView);
	//mLightManager.SetView(&mRenderTextureView);
	
	// Affichage du Level
	mRenderTexture.draw(*mLevel);
	
#if 0
	// Affichage du laser-spliter
	if (false)
	{
		sf::VertexArray splicer(sf::LinesStrip, 2);
		splicer[0].position = b22sfVec(mSpliceL1, mPhysicMgr.GetPPM());
		splicer[1].position = b22sfVec(mSpliceL2, mPhysicMgr.GetPPM());
		splicer[0].color = sf::Color::Cyan;
		splicer[1].color = sf::Color::Cyan;
		mWindow.draw(splicer);

		sf::VertexArray laser(sf::LinesStrip, 2);
		laser[0].position = b22sfVec(mSpliceP1, mPhysicMgr.GetPPM());
		laser[1].position = b22sfVec(mSpliceP2, mPhysicMgr.GetPPM());
		laser[0].color = sf::Color::Red;
		laser[1].color = sf::Color::Red;
		mWindow.draw(laser);

		sf::Sprite s(*mTextureMap["hollowCircle"]);
		s.setOrigin(u2f(s.getTexture()->getSize()) / 2.f);
		s.setPosition(b22sfVec(mSpliceP1, mPhysicMgr.GetPPM()));
		mWindow.draw(s);
		s.setPosition(b22sfVec(mSpliceP2, mPhysicMgr.GetPPM()));
		mWindow.draw(s);
	}
#endif

	// Affichage du debug
	mPhysicMgr.DrawDebugData();
	
	// Finalise le rendu des objets
	mRenderTexture.display();
	mWindow.draw(sf::Sprite(mRenderTexture.getTexture()));

#if 0
	// Création des ombres
	if (mLevel->GetLightning())
	{
		mLightManager.Update();
		mWindow.draw(mLightManager);
	}
#endif
	
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
	// Remet la vue par défaut
	mWindowView = mWindow.getDefaultView();
	mWindow.setView(mWindowView);

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
