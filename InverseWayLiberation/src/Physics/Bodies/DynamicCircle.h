#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../World.h"
#include "Body.h"

class World;
class Body;
class DynamicCircle : public Body
{
public:
	// Ctor & dtor             metres + degrès
	DynamicCircle(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float density = 1.f, float friction = 0.2f, float restitution = 0.0f
																				   , int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
	virtual ~DynamicCircle(void);
	
	// Met à jour la position du sprite
	virtual void Update();

protected:
	// Texture
	std::shared_ptr<sf::Texture> mTexture;
};
