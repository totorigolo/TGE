#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <Thor/Events.hpp>
#include <Box2D/Box2D.h>
#include <list>
#include <map>
#include "Game.h"
#include "Level.h"
#include "Physics/Body.h"
#include "Physics/World.h"
#include "Physics/MouseJoint.h"
#include "Physics/DistanceJoint.h"

class Body;
class World;
class MouseJoint;
class Box2DGame : public Game
{
public:
	// Ctor & dtor
	Box2DGame(sf::RenderWindow & window);
	virtual ~Box2DGame(void);

protected:
	/* Surcharge des fonctions évènements */
	/// Initialise le jeu
	void OnInit();

	/// Appelé quand la boucle commence
	void OnLoopBegin();

	/// Appelé pour les évènements
	void OnEvent();

	/// Appelé pour le rendu
	void OnRender();

	/// Appelé quand la boucle se termine
	void OnLoopEnd();

	// Appelé quand le jeu se termine
	void OnQuit();

private:
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
	thor::ResourceCache<sf::Texture> mTextureCache;
	std::map<std::string, std::shared_ptr<sf::Texture>> mTextureMap;
};

