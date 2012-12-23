#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <Thor/Events.hpp>
#include <Box2D/Box2D.h>
#include <list>
#include <map>
#include "../Level/Level.h"
#include "../Physics/World.h"
#include "../Physics/Entities/Ragdoll.h"
#include "../Physics/Entities/Entity.h"
#include "../Physics/Bodies/Body.h"
#include "../Physics/Bodies/DynamicBox.h"
#include "../Physics/Joints/MouseJoint.h"
#include "../Physics/Joints/DistanceJoint.h"
#include "../Resources/ResourceManager.h"
#include "../Lights/LightManager.h"
#include "../Lights/PointLight.h"

class Body;
class World;
class Ragdoll;
class Entity;
class MouseJoint;
class Box2DGame
{
public:
	// Ctor & dtor
	Box2DGame(sf::RenderWindow & window);
	virtual ~Box2DGame(void);
	
	/// Boucle de jeu
	void Run();

protected:
	/* Fonctions évènements */
	/// Initialise le jeu
	void OnInit();

	/// Appelé quand la boucle commence
	inline void OnLoopBegin();

	/// Appelé pour les évènements
	inline void OnEvent();

	/// Appelé pour la physique
	inline void OnStepPhysics();

	/// Appelé pour le rendu
	inline void OnRender();

	/// Appelé quand la boucle se termine
	inline void OnLoopEnd();

	// Appelé quand le jeu se termine
	inline void OnQuit();
	
	/* Pour la molette de la souris */
	friend class OnMouseWheelMoved;

private:
	// Etats du jeu
	bool mPaused;

	// Fenêtre
	sf::RenderWindow & mWindow;
	sf::View mWindowView;
	float mCurrentZoom;
	sf::Clock mFrameTime;
	
	// Variables pour les shaders
	sf::RenderTexture mRenderTexture;
	sf::RenderTexture mShadowRenderTexture;
	sf::View mRenderTextureView;
	sf::Shader mShader;
	sf::Clock mShaderTime;

	// Evènements
	thor::ActionMap<std::string> mActionMap;
	thor::ActionMap<std::string>::CallbackSystem mActionCallbackSystem;

	// Positions de la souris
	sf::Vector2f mCurrentMousePos;
	sf::Vector2f mCurrentMousePosRV; // RV = relative to view
	sf::Vector2f mLastMousePos;
	sf::Vector2f mLastMousePosRV; // RV = relative to view

	// Simulation physique
	sf::Clock mElapsedTime;

	// Héro
	bool mIsJumping;
	DynamicBox *mHeroBody;
	Ragdoll *mHeroRagdoll;
	Entity *mCharacter;

	// Positions de la souris (relative à la vue, en mètres et l'Y à la Box2D)
	b2Vec2 mMp;
	b2Vec2 mLMp;

	// Niveau
	Level *mLevel;

	// Monde physique
	b2Vec2 mGravity;
	World mWorld;

	// Grapin
	Body *mHookedSBody;
	b2Vec2 mHookedSAnchor;
	DistanceJoint *mHookJoint;
	sf::Clock mHookClock;

	// Déplacement des objets
	MouseJoint *mMouseJoint;

	// Découpage des objets
	bool mSplice1Get;
	b2Vec2 mSpliceL1; // Laser
	b2Vec2 mSpliceL2;
	b2Vec2 mSpliceP1; // Point d'intersection
	b2Vec2 mSpliceP2;

	// Variables pour la création de joints
	Body *mPinBodyA;
	b2Vec2 mPinAnchorA; // Ancres locales aux bodies
	Body *mPinBodyB;
	b2Vec2 mPinAnchorB;

	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Lumières
	LightManager &mLightManager;
	PointLight mMouseLight;
};

/* Pour la molette de la souris */
class OnMouseWheelMoved
{
public:
	explicit OnMouseWheelMoved(Box2DGame *_game);
	void operator() (thor::ActionContext<std::string> context);

private:
	Box2DGame *mGame;
};
