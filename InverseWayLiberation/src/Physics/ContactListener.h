#pragma once

#include <Box2D/Box2D.h>
#include "../Tools/NonCopyable.h"

class ContactListener : public b2ContactListener, public NonCopyable
{
public:
	// Début du contact (début du AABB overlap)
	void BeginContact(b2Contact* contact);

	// Fin du contact (fin du AABB overlap)
	void EndContact(b2Contact* contact);

	// Après la détection de la collision, mais avant de la résolution
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	// Après la résolution des collisions
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};
