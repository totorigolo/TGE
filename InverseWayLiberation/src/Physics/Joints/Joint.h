#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "../World.h"
#include "../Bodies/Body.h"

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

	// Gestion des joints à supprimer avant celui-ci (pour les GearJoints)
	void RegisterJoint(Joint *joint);
	void RemoveJoint(Joint *joint);
	void DestroyJoint(Joint *joint, bool remove = true);
	void DestroyAllJoints();

	// Accesseurs
	bool IsNull() const { return mIsNull; }

	World* GetWorld() { return mWorld; }
	World const* GetWorld() const { return mWorld; }

	void SetJoint(b2Joint *joint); // NE PAS UTILISER
	b2Joint* GetJoint() { return mJoint; }
	b2Joint const* GetJoint() const { return mJoint; }

	Body const* GetBodyA() const { return mBodyA; }
	Body const* GetBodyB() const { return mBodyB; }

	// Ces deux fonctions peuvent demander qqs calculs, donc à ne pas trop utiliser
	b2Vec2 GetReactionForce(float inv_dt) const;
	float GetReactionTorque(float inv_dt) const;

protected:
	bool mIsNull;

	World *mWorld;
	b2Joint *mJoint;

	Body *mBodyA;
	Body *mBodyB;

	// Liste des joint à supprimer avant celui-ci
	std::list<Joint*> mJointList;
};
