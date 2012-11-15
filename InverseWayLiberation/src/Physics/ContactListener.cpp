#include "ContactListener.h"
#include "Bodies/Body.h"
#include "Joints/Joint.h"
#include "Entities/Entity.h"
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
	if (bodyA->GetType() == BodyType::Bullet && bodyB->GetType() == BodyType::OneSidedPlatform)
	{
		actor = bodyA;
		platform = bodyB;
	}
	else if (bodyA->GetType() == BodyType::OneSidedPlatform && bodyB->GetType() == BodyType::Bullet)
	{
		actor = bodyB;
		platform = bodyA;
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
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();
	Body *bodyA = (Body*) fixtureA->GetBody()->GetUserData();
	Body *bodyB = (Body*) fixtureB->GetBody()->GetUserData();
	Entity *entity = nullptr;

	// D�gats collision Actor <> (Bad)Body
	Body *actor = nullptr, *badBody = nullptr; // :D
	if (bodyA->GetType() == BodyType::Entity)
	{
		actor = bodyA;
		badBody = bodyB;
	}
	else if (bodyB->GetType() == BodyType::Entity)
	{
		actor = bodyB;
		badBody = bodyA;
	}

	if (actor)
	{
		entity = (Entity*) actor->GetEntity();
	}

	if (actor && badBody && entity)
	{
		// R�cup�ration des dommages
		float damages = impulse->normalImpulses[0];
		if (impulse->count == 2)
		{
			damages += impulse->normalImpulses[1];
		}

		// Annonce :)
		// D�gats de chute
		if (damages >= 8.f)
		{
			std::cout << "Et BOUM ! " << damages << " PV en moins !" << std::endl;
			entity->AddDamage(damages);
		}
		// D�gats de contact
		else if (badBody->GetBody()->GetType() == b2_dynamicBody)// && damages >= 1.5f)
		{
			std::cout << "Et BAM ! " << damages << " PV en moins !" << std::endl;
			entity->AddDamage(damages);
		}
	}
}