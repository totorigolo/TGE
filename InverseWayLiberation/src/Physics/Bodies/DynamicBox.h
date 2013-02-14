#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../World.h"
#include "../Body.h"

class World;
class Body;
class DynamicBox : public Body
{
public:
	// Ctor & dtor          metres + degrès
	DynamicBox(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture>, float density = 1.f, float friction = 0.2f, float restitution = 0.0f
																		, int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
	DynamicBox(World *world, b2Vec3 posRot, sf::Sprite *sprite, float density = 1.f, float friction = 0.2f, float restitution = 0.0f
															  , int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
	virtual ~DynamicBox(void);
	
	// Met à jour la position du sprite
	virtual void Update();

	// Change les collisions / la taille du body
	void SetSize(float w, float h);

	// Accesseur
	virtual b2AABB GetBodyAABB() const;

private:
};
