#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "Body.h"

class StaticBox : public Body
{
public:
	// Ctor & dtor
	StaticBox(b2World *world, b2Vec2 posInMeters, std::shared_ptr<sf::Texture> texture);
	virtual ~StaticBox(void);
	
	// Crée le cercle dans le monde
	virtual void Create(b2Vec2 posInMeters);

private:
};
