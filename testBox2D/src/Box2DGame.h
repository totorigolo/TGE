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
	// Positions de la souris (relative � la vue, non invers�e et en metres)
	b2Vec2 mMp;
	b2Vec2 mLMp;

	// Monde physique
	b2Vec2 mGravity;
	b2World mWorld;
	std::list<Body*> mBodyList;
	std::list<Body*> mStaticBodyList;

	// D�placement des objets
	b2MouseJoint *mMouseJoint;

	// Textures
	thor::ResourceCache<sf::Texture> mTextureCache;
	std::map<std::string, std::shared_ptr<sf::Texture>> mTextureMap;
};

