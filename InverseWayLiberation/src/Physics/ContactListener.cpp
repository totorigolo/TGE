#include "ContactListener.h"
#include "Body.h"
#include "Joint.h"
#include "../Entities/Entity.h"
#include "../Entities/Player.h"
#include <iostream>

// D�but du contact (d�but du AABB overlap)
void ContactListener::BeginContact(b2Contact* contact)
{
	/*
	contact->GetManifold(); // Les points de contact
	contact->IsTouching(); // Est-ce que les shapes se touchent effectivement
	contact->SetEnabled(true); // Activer / D�sactiver le contact
	contact->GetFixtureA(); // Le 1er shape
	contact->GetFixtureB(); // Le 2nd shape
	contact->GetFriction(); // Obtenir & modifier les propri�t�s
	contact->GetRestitution();
	//*/
	
	// R�cup�re les bodies
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();
	Body *bodyA = (Body*) fixtureA->GetBody()->GetUserData();
	Body *bodyB = (Body*) fixtureB->GetBody()->GetUserData();

	// Si il s'agit d'un LivingBeing, on lui dit qu'il peut sauter
	if ((bodyA->GetEntity()->GetType() == EntityType::Player || bodyA->GetEntity()->GetType() == EntityType::LivingBeing)
		&& contact->IsTouching())
	{
		// Si le contact est SOUS le perso
		if (contact->GetManifold()->localPoint.y >= bodyA->GetBodyPosition().y + (bodyA->GetBodySize().y / 2.f))
		{
			((LivingBeing*) bodyA->GetEntity())->CanJump(true);
		}
	}
	else if ((bodyB->GetEntity()->GetType() == EntityType::Player || bodyB->GetEntity()->GetType() == EntityType::LivingBeing)
		&& contact->IsTouching())
	{
		// Si le contact est SOUS le perso
		if (contact->GetManifold()->localPoint.y >= bodyA->GetBodyPosition().y + (bodyA->GetBodySize().y / 2.f))
		{
			((LivingBeing*) bodyB->GetEntity())->CanJump(true);
		}
	}
}

// Fin du contact (fin du AABB overlap)
void ContactListener::EndContact(b2Contact* contact)
{
	/*
	contact->GetManifold(); // Les points de contact
	contact->IsTouching(); // Est-ce que les shapes se touchent effectivement
	contact->SetEnabled(true); // Activer / D�sactiver le contact
	contact->GetFixtureA(); // Le 1er shape
	contact->GetFixtureB(); // Le 2nd shape
	contact->GetFriction(); // Obtenir & modifier les propri�t�s
	contact->GetRestitution();
	//*/

	// R�cup�re les bodies
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();
	Body *bodyA = (Body*) fixtureA->GetBody()->GetUserData();
	Body *bodyB = (Body*) fixtureB->GetBody()->GetUserData();

	// Si il s'agit d'un LivingBeing, on lui dit qu'il ne peut plus sauter
	if (bodyA->GetEntity()->GetType() == EntityType::Player || bodyA->GetEntity()->GetType() == EntityType::LivingBeing)
	{
		((LivingBeing*) bodyA->GetEntity())->CanJump(false);
	}
	else if (bodyB->GetEntity()->GetType() == EntityType::Player || bodyB->GetEntity()->GetType() == EntityType::LivingBeing)
	{
		((LivingBeing*) bodyB->GetEntity())->CanJump(false);
	}
}

// Apr�s la d�tection de la collision, mais avant la r�solution
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	// R�cup�re les bodies
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();
	Body *bodyA = (Body*) fixtureA->GetBody()->GetUserData();
	Body *bodyB = (Body*) fixtureB->GetBody()->GetUserData();

	// Collision Actor <> OneSidedPlatform
	Body *actor = nullptr, *platform = nullptr;
	if (bodyA->GetType() == BodyType::OneSidedPlatform)
	{
		actor = bodyB;
		platform = bodyA;
	}
	else if (bodyB->GetType() == BodyType::OneSidedPlatform)
	{
		actor = bodyA;
		platform = bodyB;
	}
	if (actor && platform)
	{
		if (actor->GetBody()->GetLinearVelocity().y < 5.f && actor->GetBody()->GetLinearVelocity().y > 0.1f
			|| actor->GetBodyAABB().lowerBound.y < platform->GetBodyAABB().upperBound.y - 0.2f)
		{
			contact->SetEnabled(false);
		}
	}
}

// Apr�s la r�solution des collisions
void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	/*
	contact->GetManifold(); // Les points de contact
	contact->IsTouching(); // Est-ce que les shapes se touchent effectivement
	contact->SetEnabled(true); // Activer / D�sactiver le contact
	contact->GetFixtureA(); // Le 1er shape
	contact->GetFixtureB(); // Le 2nd shape
	contact->GetFriction(); // Obtenir & modifier les propri�t�s
	contact->GetRestitution();
	
	impulse->count; // Le nombre d'impultions
	impulse->normalImpulses;
	impulse->tangentImpulses;
	/*
	http://blog.allanbishop.com/box-2d-2-1a-tutorial-part-6-collision-strength/
    - Normal impulse:
	    The normal force is the force applied at a contact point to prevent the shapes from penetrating.
	    For convenience, Box2D works with impulses. The normal impulse is just the normal force multiplied by the time step.
    - Tangent impulse:
        The tangent force is generated at a contact point to simulate friction. For convenience, this is stored as an impulse.
	*/
	//*/
}