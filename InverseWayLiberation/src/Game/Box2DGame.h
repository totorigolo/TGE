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
	/* Fonctions �v�nements */
	/// Initialise le jeu
	void OnInit();

	/// Appel� quand la boucle commence
	inline void OnLoopBegin();

	/// Appel� pour les �v�nements
	inline void OnEvent();

	/// Appel� pour la physique
	inline void OnStepPhysics();

	/// Appel� pour le rendu
	inline void OnRender();

	/// Appel� quand la boucle se termine
	inline void OnLoopEnd();

	// Appel� quand le jeu se termine
	inline void OnQuit();
	
	/* Pour la molette de la souris */
	friend class OnMouseWheelMoved;

private:
	// Etats du jeu
	bool mPaused;

	// Fen�tre
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

	// Ev�nements
	thor::ActionMap<std::string> mActionMap;
	thor::ActionMap<std::string>::CallbackSystem mActionCallbackSystem;

	// Positions de la souris
	sf::Vector2f mCurrentMousePos;
	sf::Vector2f mCurrentMousePosRV; // RV = relative to view
	sf::Vector2f mLastMousePos;
	sf::Vector2f mLastMousePosRV; // RV = relative to view

	// Simulation physique
	sf::Clock mElapsedTime;

	// H�ro
	bool mIsJumping;
	DynamicBox *mHeroBody;
	Ragdoll *mHeroRagdoll;
	Entity *mCharacter;

	// Positions de la souris (relative � la vue, en m�tres et l'Y � la Box2D)
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

	// D�placement des objets
	MouseJoint *mMouseJoint;

	// D�coupage des objets
	bool mSplice1Get;
	b2Vec2 mSpliceL1; // Laser
	b2Vec2 mSpliceL2;
	b2Vec2 mSpliceP1; // Point d'intersection
	b2Vec2 mSpliceP2;

	// Variables pour la cr�ation de joints
	Body *mPinBodyA;
	b2Vec2 mPinAnchorA; // Ancres locales aux bodies
	Body *mPinBodyB;
	b2Vec2 mPinAnchorB;

	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Lumi�res
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
