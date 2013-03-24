#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "../Resources/ResourceManager.h"
#include "../Physics/PhysicManager.h"
#include "../Tools/NonCopyable.h"
#include "../Entities/Grapnel.h"
#include "../Level/Level.h"
#include "InputManager.h"

class Box2DGame : public NonCopyable
{
public:
	// Ctor & dtor
	Box2DGame(sf::RenderWindow &window);
	virtual ~Box2DGame(void);
	
	/// Boucle de jeu
	void Run();

protected:
	/* Fonctions �v�nements */
	/// Initialise le jeu
	inline bool OnInit();

	/// Appel� quand la boucle commence
	inline void OnLoopBegin();

	/// Appel� pour les �v�nements
	inline void OnEvent();
	
	/// Appel� pour la logique
	inline void OnLogic();

	/// Appel� pour la physique
	inline void OnStepPhysics();
	
	/// Appel� pour les mises � jour
	inline void OnUpdate();

	/// Appel� pour le rendu
	inline void OnRender();

	/// Appel� quand la boucle se termine
	inline void OnLoopEnd();

	// Appel� quand le jeu se termine
	inline void OnQuit();
	
private:
	// Etats du jeu
	bool mPaused;

	// Fen�tre
	bool mQuit;
	sf::RenderWindow & mWindow;
	
	// Ev�nements
	InputManager &mInputManager;

	// Niveau
	Level *mLevel;

	// Monde physique
	b2Vec2 mGravity;
	PhysicManager mPhysicMgr;

	// Textures
	ResourceManager &mResourceManager;
	TextureMap &mTextureMap;

	// Positions de la souris
	b2Vec2 mMp;

	// Grapin
	b2Body *mHookedSBody;
	b2Vec2 mHookedSAnchor;
	Grapnel *mGrapnel;
	sf::Clock mHookClock;

	// D�placement des objets
	int mMouseJointID;

	// Variables pour la cr�ation de joints
	b2Body *mPinBodyA;
	b2Vec2 mPinAnchorA; // Ancres locales aux bodies
	b2Body *mPinBodyB;
	b2Vec2 mPinAnchorB;

};
