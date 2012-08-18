#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Events.hpp>
#include <Thor/Resources.hpp>
#include <Box2D/Box2D.h>
#include <list>
#include <map>
#include "Game.h"
#include "Body.h"

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
	// Positions de la souris (relative à la vue, non inversée et en metres)
	b2Vec2 mMp;
	b2Vec2 mLMp;

	// Monde physique
	b2Vec2 mGravity;
	b2World mWorld;
	std::list<Body*> mBodyList;
	std::list<Body*> mStaticBodyList;

	// Déplacement des objets
	b2MouseJoint *mMouseJoint;

	// Textures
	thor::ResourceCache<sf::Texture> mTextureCache;
	std::map<std::string, std::shared_ptr<sf::Texture>> mTextureMap;
};

