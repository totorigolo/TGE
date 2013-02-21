#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include "PhysicManager.h"

class PhysicManager;
class Joint : public sf::VertexArray
{
public:
	// Ctor & dtor
	Joint(PhysicManager *physicMgr);
	virtual ~Joint(void);

	// Destruction de ce joint
	void Destroy();

	// Met � jour le joint
	virtual void Update();

	// Gestion des joints � supprimer avant celui-ci (pour les GearJoints)
	void RegisterJoint(Joint *joint);
	void RemoveJoint(Joint *joint);
	void DestroyJoint(Joint *joint, bool remove = true);
	void DestroyAllJoints();

	// D�finit si le joint est cassable
	void SetBreakableByForce(bool breakable);
	void SetMaxForce(b2Vec2 maxForce);
	void SetMaxForce(float maxForce);
	void SetBreakableByTorque(bool breakable);
	void SetMaxTorque(float maxTorque);
	bool IsBreakableMaxForce() const;
	float GetMaxForce() const;
	b2Vec2 GetMaxVecForce() const;
	bool IsBreakableMaxTorque() const;
	float GetMaxTorque() const;

	// Accesseurs
	bool IsNull() const { return mIsNull; }

	PhysicManager* GetPhysicManager() { return mPhysicMgr; }
	PhysicManager const* GetPhysicManager() const { return mPhysicMgr; }

	void SetJoint(b2Joint *joint); // NE PAS UTILISER
	b2Joint* GetJoint() { return mJoint; }
	b2Joint const* GetJoint() const { return mJoint; }

	const b2Body* GetBodyA() const { return mBodyA; }
	const b2Body* GetBodyB() const { return mBodyB; }

	// Ces deux fonctions peuvent demander qqs calculs, donc � ne pas trop utiliser
	b2Vec2 GetReactionForce(float inv_dt) const;
	float GetReactionTorque(float inv_dt) const;

protected:
	// Propri�t�s
	bool mIsNull;

	// Objets physiques
	PhysicManager *mPhysicMgr;
	b2Joint *mJoint;

	// Les b2Bodies attach�es
	b2Body *mBodyA;
	b2Body *mBodyB;

	// Propri�t�s cassable
	bool mIsBreakableMaxForce;
	enum { Null, Float, Vector } mMaxForceType;
	float mMaxForce;
	b2Vec2 mMaxVecForce;
	bool mIsBreakableMaxTorque;
	float mMaxTorque;

	// Liste des joint � supprimer avant celui-ci
	std::list<Joint*> mJointList;
};
