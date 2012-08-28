#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <Thor/Events.hpp>
#include <Box2D/Box2D.h>
#include <list>
#include <map>
#include "Game.h"
#include "Level.h"
#include "Physics/World.h"
#include "Physics/Ragdoll.h"
#include "Physics/Bodies/Body.h"
#include "Physics/Joints/MouseJoint.h"
#include "Physics/Joints/DistanceJoint.h"

class Body;
class World;
class Ragdoll;
class MouseJoint;
class Box2DGame : public Game
{
public:
	// Ctor & dtor
	Box2DGame(sf::RenderWindow & window);
	virtual ~Box2DGame(void);

protected:
	/* Surcharge des fonctions �v�nements */
	/// Initialise le jeu
	void OnInit();

	/// Appel� quand la boucle commence
	void OnLoopBegin();

	/// Appel� pour les �v�nements
	void OnEvent();

	/// Appel� pour le rendu
	void OnRender();

	/// Appel� quand la boucle se termine
	void OnLoopEnd();

	// Appel� quand le jeu se termine
	void OnQuit();

private:
	// Variables pour les shaders
	sf::RenderTexture mRenderTexture;
	sf::Shader mShader;
	sf::Clock mShaderTime;

	// Positions de la souris (relative � la vue, en m�tres et l'Y � la Box2D)
	b2Vec2 mMp;
	b2Vec2 mLMp;

	// Niveau
	Level *mLevel;

	// Monde physique
	b2Vec2 mGravity;
	World mWorld;

	// H�ro
	Ragdoll *mHeroRagdoll;
	
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
	thor::ResourceCache<sf::Texture> mTextureCache;
	std::map<std::string, std::shared_ptr<sf::Texture>> mTextureMap;
};

