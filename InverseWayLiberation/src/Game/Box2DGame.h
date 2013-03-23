#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <list>
#include <map>
#include "../Level/Level.h"
#include "../Tools/NonCopyable.h"
#include "../Physics/Joints/MouseJoint.h"
#include "../Resources/ResourceManager.h"
#include "../Physics/PhysicManager.h"
#include "../Entities/Grapnel.h"
#include "InputManager.h"

class MouseJoint;
class Box2DGame : public NonCopyable
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
	inline bool OnInit();

	/// Appelé quand la boucle commence
	inline void OnLoopBegin();

	/// Appelé pour les évènements
	inline void OnEvent();
	
	/// Appelé pour la logique
	inline void OnLogic();

	/// Appelé pour la physique
	inline void OnStepPhysics();
	
	/// Appelé pour les mises à jour
	inline void OnUpdate();

	/// Appelé pour le rendu
	inline void OnRender();

	/// Appelé quand la boucle se termine
	inline void OnLoopEnd();

	// Appelé quand le jeu se termine
	inline void OnQuit();
	
private:
	// Etats du jeu
	bool mPaused;

	// Fenêtre
	bool mQuit;
	sf::RenderWindow & mWindow;
	sf::Clock mFrameTime;
	
	// Evènements
	InputManager &mInputManager;

	// Niveau
	Level *mLevel;

	// Monde physique
	b2Vec2 mGravity;
	//World mWorld;
	PhysicManager mPhysicMgr;

	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Variables pour les shaders
	sf::RenderTexture mRenderTexture;
	sf::RenderTexture mShadowRenderTexture;
	sf::View mRenderTextureView;
	sf::Shader mShader;
	sf::Clock mShaderTime;

	// Simulation physique
	sf::Clock mElapsedTime;

	// Positions de la souris (relative à la vue, en mètres et l'Y à la Box2D)
	b2Vec2 mMp;

	// Grapin
	b2Body *mHookedSBody;
	b2Vec2 mHookedSAnchor;
	Grapnel *mGrapnel;
	sf::Clock mHookClock;

	// Déplacement des objets
	int mMouseJointID;

	// Découpage des objets
	bool mSplice1Get;
	b2Vec2 mSpliceL1; // Laser
	b2Vec2 mSpliceL2;
	b2Vec2 mSpliceP1; // Point d'intersection
	b2Vec2 mSpliceP2;

	// Variables pour la création de joints
	b2Body *mPinBodyA;
	b2Vec2 mPinAnchorA; // Ancres locales aux bodies
	b2Body *mPinBodyB;
	b2Vec2 mPinAnchorB;

};
