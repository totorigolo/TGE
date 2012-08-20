#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "World.h"
#include "Body.h"

class World;
class Body;
class StaticBox : public Body
{
public:
	// Ctor & dtor
	StaticBox(World *world, b2Vec2 posInMeters, std::shared_ptr<sf::Texture> texture);
	virtual ~StaticBox(void);
	
	// Met à jour la position du sprite
	virtual void Update();

protected:
	// Texture
	std::shared_ptr<sf::Texture> mTexture;
};
