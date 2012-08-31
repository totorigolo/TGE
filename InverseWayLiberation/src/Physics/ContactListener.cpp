#include "ContactListener.h"
#include "Bodies/Body.h"
#include "Joints/Joint.h"
#include <iostream>
// D�but du contact (d�but du AABB overlap)
void ContactListener::BeginContact(b2Contact* contact)
{
}

// Fin du contact (fin du AABB overlap)
void ContactListener::EndContact(b2Contact* contact)
{
}

// Apr�s la d�tection de la collision, mais avant de la r�solution
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();
	Body *bodyA = (Body*) fixtureA->GetBody()->GetUserData();
	Body *bodyB = (Body*) fixtureB->GetBody()->GetUserData();

	// Collision Actor <> OneSidedPlatform
	Body *actor = nullptr, *platform = nullptr;
	if (bodyA->GetType() == body_actor && bodyB->GetType() == body_oneSidedPlatform)
	{
		actor = bodyA;
		platform = bodyB;
	}
	else if (bodyA->GetType() == body_oneSidedPlatform && bodyB->GetType() == body_actor)
	{
		actor = bodyB;
		platform = bodyA;
	}
	if (actor && platform)
	{
		// TODO: Amm�liorer la m�thode
		if (/*actor->GetBody()->GetLinearVelocity().y > 0.f || */actor->GetBodyAABB().lowerBound.y < platform->GetBodyAABB().upperBound.y - 0.08f)
		{
			contact->SetEnabled(false);
		}
	}
}

// Apr�s la r�solution des collisions
void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();
	Body *bodyA = (Body*) fixtureA->GetBody()->GetUserData();
	Body *bodyB = (Body*) fixtureB->GetBody()->GetUserData();

	// Collision Actor <> OneSidedPlatform
	Body *actor = nullptr, *badBody = nullptr; // :D
	if (bodyA->GetType() == body_actor)
	{
		actor = bodyA;
		badBody = bodyB;
	}
	else if (bodyB->GetType() == body_actor)
	{
		actor = bodyB;
		badBody = bodyA;
	}
	if (actor && badBody)
	{

	}
}