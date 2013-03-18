#include "ContactListener.h"
#include "Joint.h"
#include "../Entities/Entity.h"
#include "../Entities/Player.h"
#include "../Entities/BasicBody.h"
#include <iostream>

// Début du contact (début du AABB overlap)
void ContactListener::BeginContact(b2Contact* contact)
{
	/*
	contact->GetManifold(); // Les points de contact
	contact->IsTouching(); // Est-ce que les shapes se touchent effectivement
	contact->SetEnabled(true); // Activer / Désactiver le contact
	contact->GetFixtureA(); // Le 1er shape
	contact->GetFixtureB(); // Le 2nd shape
	contact->GetFriction(); // Obtenir & modifier les propriétés
	contact->GetRestitution();
	//*/
	
	// Récupère les bodies
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();
	Entity *entityA = (Entity*) fixtureA->GetBody()->GetUserData();
	Entity *entityB = (Entity*) fixtureB->GetBody()->GetUserData();

	// Si il s'agit d'un LivingBeing, on lui dit qu'il peut sauter
	/*if ((entityA->GetType() == EntityType::Player || entityA->GetType() == EntityType::LivingBeing)
		&& contact->IsTouching())
	{
		// Si le contact est SOUS le perso
		if (contact->GetManifold()->localPoint.y >= bodyA->GetBody()->GetPosition().y + (bodyA->GetBody()->GetSize().y / 2.f))
		{
			((LivingBeing*) bodyA)->CanJump(true);
		}
	}
	else if ((entityB->GetEntity()->GetType() == EntityType::Player || entityB->GetBody()->GetType() == EntityType::LivingBeing)
		&& contact->IsTouching())
	{
		// Si le contact est SOUS le perso
		if (contact->GetManifold()->localPoint.y >= bodyA->GetBodyPosition().y + (bodyA->GetBodySize().y / 2.f))
		{
			((LivingBeing*) bodyB->GetEntity())->CanJump(true);
		}
	}*/
}

// Fin du contact (fin du AABB overlap)
void ContactListener::EndContact(b2Contact* contact)
{
	/*
	contact->GetManifold(); // Les points de contact
	contact->IsTouching(); // Est-ce que les shapes se touchent effectivement
	contact->SetEnabled(true); // Activer / Désactiver le contact
	contact->GetFixtureA(); // Le 1er shape
	contact->GetFixtureB(); // Le 2nd shape
	contact->GetFriction(); // Obtenir & modifier les propriétés
	contact->GetRestitution();
	//*/

	// Récupère les bodies
	/*b2Fixture *fixtureA = contact->GetFixtureA();
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
	}*/
}

// Après la détection de la collision, mais avant la résolution
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	// Récupère les bodies
	Entity *entityA = (Entity*) contact->GetFixtureA()->GetBody()->GetUserData();
	Entity *entityB = (Entity*) contact->GetFixtureB()->GetBody()->GetUserData();

	// Traite les BasicBodies
	if (entityA->GetType() == EntityType::BasicBody)
	{
		/* Collision Actor <> OneSidedPlatform */
		// Récupère les acteurs
		b2Fixture *actor = nullptr, *platform = nullptr;
		if (((BasicBody*) entityA)->GetCollisionType() == BasicBody::CollisionType::OneSidedPlatform)
		{
			actor = contact->GetFixtureB();
			platform = contact->GetFixtureA();
		}
		else if (((BasicBody*) entityB)->GetCollisionType() == BasicBody::CollisionType::OneSidedPlatform)
		{
			actor = contact->GetFixtureA();
			platform = contact->GetFixtureB();
		}
		
		// Si on a les deux acteurs
		if (actor && platform)
		{
			if (actor->GetBody()->GetLinearVelocity().y < 5.f && actor->GetBody()->GetLinearVelocity().y > 0.1f
				|| actor->GetAABB(0).lowerBound.y < platform->GetAABB(0).upperBound.y - 0.2f)
			{
				contact->SetEnabled(false);
			}
		}
	}
}

// Après la résolution des collisions
void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	/*
	contact->GetManifold(); // Les points de contact
	contact->IsTouching(); // Est-ce que les shapes se touchent effectivement
	contact->SetEnabled(true); // Activer / Désactiver le contact
	contact->GetFixtureA(); // Le 1er shape
	contact->GetFixtureB(); // Le 2nd shape
	contact->GetFriction(); // Obtenir & modifier les propriétés
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