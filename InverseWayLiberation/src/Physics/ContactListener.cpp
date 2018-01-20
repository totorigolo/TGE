#include "ContactListener.h"
#include "../Entities/Entity.h"
#include "../Entities/BasicBody.h"

// Début du contact (début du AABB overlap)
void ContactListener::BeginContact(b2Contact *contact) {
    B2_NOT_USED(contact);
    /*
    contact->GetManifold(); // Les points de contact
    contact->IsTouching(); // Est-ce que les shapes se touchent effectivement
    contact->SetEnabled(true); // Activer / Désactiver le contact
    contact->GetFixtureA(); // Le 1er shape
    contact->GetFixtureB(); // Le 2nd shape
    contact->GetFriction(); // Obtenir & modifier les propriétés
    contact->GetRestitution();
    //*/

    /*/ Récupère les bodies
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();
    Entity *entityA = (Entity*) fixtureA->GetBody()->GetUserData();
    Entity *entityB = (Entity*) fixtureB->GetBody()->GetUserData();
    //*/
}

// Fin du contact (fin du AABB overlap)
void ContactListener::EndContact(b2Contact *contact) {
    B2_NOT_USED(contact);
    /*
    contact->GetManifold(); // Les points de contact
    contact->IsTouching(); // Est-ce que les shapes se touchent effectivement
    contact->SetEnabled(true); // Activer / Désactiver le contact
    contact->GetFixtureA(); // Le 1er shape
    contact->GetFixtureB(); // Le 2nd shape
    contact->GetFriction(); // Obtenir & modifier les propriétés
    contact->GetRestitution();
    //*/

    /*/ Récupère les bodies
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();
    Entity *entityA = (Entity*) fixtureA->GetBody()->GetUserData();
    Entity *entityB = (Entity*) fixtureB->GetBody()->GetUserData();
    //*/
}

// Après la détection de la collision, mais avant la résolution
void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
    B2_NOT_USED(oldManifold);

    // Récupère les bodies
    Entity * entityA = (Entity *) contact->GetFixtureA()->GetBody()->GetUserData();
    myAssert(entityA, "Contact avec un b2Body sans Entity associée.");


    // Traite les BasicBodies
    if (entityA->GetType() == EntityType::BasicBody) {
        /* Collision b2Body <> OneSidedPlatform */
        if (((BasicBody *) entityA)->GetCollisionType() == BasicBody::CollisionType::OneSidedPlatform) {
            // Récupère les acteurs
            b2Fixture *actor = contact->GetFixtureB();
            b2Fixture *platform = contact->GetFixtureA();

            // Vérifie si les conditions sont là pour annuler le contact
            if ((actor->GetBody()->GetLinearVelocity().y < 5.f && actor->GetBody()->GetLinearVelocity().y > 0.1f)
                || actor->GetAABB(0).lowerBound.y < platform->GetAABB(0).upperBound.y - 0.2f) {
                contact->SetEnabled(false);
            }
        }
    }
}

// Après la résolution des collisions
void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
    B2_NOT_USED(contact);
    B2_NOT_USED(impulse);

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
    / *
    http://blog.allanbishop.com/box-2d-2-1a-tutorial-part-6-collision-strength/
    - Normal impulse:
        The normal force is the force applied at a contact point to prevent the shapes from penetrating.
        For convenience, Box2D works with impulses. The normal impulse is just the normal force multiplied by the time step.
    - Tangent impulse:
        The tangent force is generated at a contact point to simulate friction. For convenience, this is stored as an impulse.
    */
    //*/
}