#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "World.h"
#include "Body.h"

class World;
class Body;
class Joint : public sf::VertexArray
{
public:
	// Ctor & dtor
	Joint(World *world);
	virtual ~Joint(void);

	// Met à jour le joint
	virtual void Update();

	// Accesseurs
	World* GetWorld() { return mWorld; }
	World const* GetWorld() const { return mWorld; }

	void SetJoint(b2Joint *joint); // NE PAS UTILISER
	b2Joint* GetJoint() { return mJoint; }
	b2Joint const* GetJoint() const { return mJoint; }

	Body const* GetBodyA() const { return mBodyA; }
	Body const* GetBodyB() const { return mBodyB; }
	b2Vec2 const& GetAnchorA() const { return mAnchorA; }
	b2Vec2 const& GetAnchorB() const { return mAnchorB; }

protected:
	World *mWorld;
	b2Joint *mJoint;

	Body *mBodyA;
	Body *mBodyB;
	b2Vec2 mAnchorA;
	b2Vec2 mAnchorB;
};
