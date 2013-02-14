#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../World.h"
#include "../Body.h"

class World;
class Body;
class KinematicBox : public Body
{
public:
	// Ctor & dtor          metres + degrès
	KinematicBox(World *world, b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float restitution
																				  , int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
	virtual ~KinematicBox(void);
	
	// Met à jour la position du sprite
	virtual void Update();

	// Accesseurs
	virtual b2AABB GetBodyAABB() const;

	virtual void SetVelocity(b2Vec2 const& velocity, float angularVelocity = 0.f);
	virtual void SetPosition(b2Vec2 const& pos, float angle);

private:
};
