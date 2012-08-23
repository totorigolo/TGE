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
	// Ctor & dtor         metres + degr�s
	StaticBox(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float friction = 0.2f, float restitution = 0.f);
	virtual ~StaticBox(void);
	
	// Met � jour la position du sprite
	virtual void Update();

protected:
	// Texture
	std::shared_ptr<sf::Texture> mTexture;
};