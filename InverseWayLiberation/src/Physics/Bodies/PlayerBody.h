#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../World.h"
#include "../Body.h"

class World;
class Body;
class PlayerBody : public Body
{
public:
	// Ctor & dtor          metres + degrès
	PlayerBody(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture>);
	virtual ~PlayerBody(void);
	
	// Met à jour la position du sprite
	virtual void Update();

	// Change les collisions / la taille du body
	void SetSize(float w, float h);

	// Accesseur
	virtual b2AABB GetBodyAABB() const;

private:
};
