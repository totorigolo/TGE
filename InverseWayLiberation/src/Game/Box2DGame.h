#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "../Resources/ResourceManager.h"
#include "../Physics/PhysicManager.h"
#include "../Tools/NonCopyable.h"
#include "../Entities/Grapnel.h"
#include "../Level/LevelManager.h"
#include "InputManager.h"
#include "LuaMachine.h"

class Box2DGame : public NonCopyable
{
public:
	// Ctor & dtor
	Box2DGame(sf::RenderWindow &window);
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
	sf::RenderWindow &mWindow;
	
	// Evènements
	InputManager &mInputManager;

	// Niveau
	LevelManager &mLevel;

	// Monde physique
	PhysicManager &mPhysicMgr;

	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Machine Lua
	LuaMachine mConsole;

	// Positions de la souris
	b2Vec2 mMp;

	// Grapin
	b2Body *mHookedSBody;
	b2Vec2 mHookedSAnchor;
	Grapnel *mGrapnel;
	sf::Clock mHookClock;

	// Déplacement des objets
	bool mMouseJointCreated;
	int mMouseJointID;

	// Variables pour la création de joints
	b2Body *mPinBodyA;
	b2Vec2 mPinAnchorA; // Ancres locales aux bodies
	b2Body *mPinBodyB;
	b2Vec2 mPinAnchorB;

};
