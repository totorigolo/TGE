#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <Box2D/Box2D.h>
#include "../Tools/NonCopyable.h"

class ContactListener : public b2ContactListener, public NonCopyable
{
public:
	// D�but du contact (d�but du AABB overlap)
	void BeginContact(b2Contact* contact);

	// Fin du contact (fin du AABB overlap)
	void EndContact(b2Contact* contact);

	// Apr�s la d�tection de la collision, mais avant de la r�solution
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	// Apr�s la r�solution des collisions
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};
