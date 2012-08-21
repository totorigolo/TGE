#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "World.h"
#include "Body.h"

class World;
class Body;
class DynamicBox : public Body
{
public:
	// Ctor & dtor
	DynamicBox(World *world, b2Vec2 pos, std::shared_ptr<sf::Texture> texture, float density = 1.f, float friction = 0.2f, float restitution = 0.0f);
	virtual ~DynamicBox(void);
	
	// Met à jour la position du sprite
	virtual void Update();

private:
	// Texture
	std::shared_ptr<sf::Texture> mTexture;
};
