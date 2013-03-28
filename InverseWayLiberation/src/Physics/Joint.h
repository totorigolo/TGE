#pragma once
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include <list>
#include "PhysicManager.h"
#include "../Entities/Entity.h"
#include "../Tools/NonCopyable.h"

class PhysicManager;
class Joint : public NonCopyable
{
	friend class PhysicManager;

public:
	// Ctor & dtor
	Joint();
	virtual ~Joint(void);

	// Destruction de ce joint
	void Destroy();

	// Met à jour le joint
	virtual void Update();

	// Gestion des joints à supprimer avant celui-ci (pour les GearJoints surtout)
	void RegisterLinkedJoint(int jointID);
	void RemoveLinkedJoint(int jointID);
	void DestroyLinkedJoint(int jointID);
	void DestroyAllLinkedJoints();

	// Définit si le joint est cassable
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
	bool IsAlive() const { return mIsAlive; }
	bool ToDestroy() const { return mToDestroy; }

	int GetID() const { return mID; }

	b2Joint* GetJoint() { return mJoint; }
	b2Joint const* GetJoint() const { return mJoint; }
	b2JointType GetJointType() const { return mJoint->GetType(); }
	
	b2Body* GetBodyA();
	b2Body* GetBodyB();
	const b2Body* GetBodyA() const;
	const b2Body* GetBodyB() const;

	// Ces deux fonctions peuvent demander qqs calculs, donc à ne pas trop utiliser
	b2Vec2 GetReactionForce(float inv_dt) const;
	float GetReactionTorque(float inv_dt) const;

protected:
	// Etat
	bool mIsAlive;
	bool mToDestroy;

	// Objets physiques
	int mID;
	PhysicManager &mPhysicMgr;
	b2Joint *mJoint;

	// Propriétés cassable
	bool mIsBreakableMaxForce;
	enum { Null, Float, Vector } mMaxForceType;
	float mMaxForce;
	b2Vec2 mMaxVecForce;
	bool mIsBreakableMaxTorque;
	float mMaxTorque;

	// Liste des joint à supprimer avant celui-ci
	std::list<int> mLinkedJointList;
};
