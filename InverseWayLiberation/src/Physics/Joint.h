#pragma once
#include "../Tools/NonCopyable.h"

#include <Box2D/Box2D.h>
#include <list>

enum class ForceType
{
	Null,
	Float,
	Vector
};

enum class JointType
{
	Null,
	DistanceJoint,
	FrictionJoint,
	GearJoint,
	MouseJoint,
	PrismaticJoint,
	PulleyJoint,
	RevoluteJoint,
	RopeJoint,
	WeldJoint,
	WheelJoint
};

struct JointDef
{
	// Ctor
	JointDef()
	{
		isBreakableMaxForce = false;
		maxForceType = ForceType::Null;
		maxForce = 0.f;
		maxVecForce = b2Vec2_zero;
		isBreakableMaxTorque = false;
		maxTorque = 0.f;
	}

	// Propriétés cassable
	bool isBreakableMaxForce;
	ForceType maxForceType;
	float maxForce;
	b2Vec2 maxVecForce;
	bool isBreakableMaxTorque;
	float maxTorque;
};

class PhysicManager;
class Joint : public NonCopyable
{
	friend class PhysicManager;

public:
	// Ctor & dtor
	Joint();
	Joint(const JointDef &def);
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
	const std::list<int>& GetLinkedJoints();

	// Définit si le joint est cassable
	void SetBreakableByForce(bool breakable);
	void SetMaxForce(b2Vec2 maxForce);
	void SetMaxForce(float maxForce);
	void SetBreakableByTorque(bool breakable);
	void SetMaxTorque(float maxTorque);
	bool IsBreakableMaxForce() const;
	ForceType GetMaxForceType() const;
	float GetMaxForce() const;
	b2Vec2 GetMaxVecForce() const;
	bool IsBreakableMaxTorque() const;
	float GetMaxTorque() const;

	// Accesseurs
	bool IsAlive() const { return mIsAlive; }
	bool ToDestroy() const { return mToDestroy; }

	JointType GetType() const;
	int GetID() const { return mID; }

	b2Joint* GetJoint() { return mJoint; }
	b2Joint const* GetJoint() const { return mJoint; }
	b2JointType GetJointType() const { return mJoint->GetType(); }
	
	b2Body* GetBodyA();
	b2Body* GetBodyB();
	const b2Body* GetBodyA() const;
	const b2Body* GetBodyB() const;
	bool IsCollideConnected() const;

	// Ces deux fonctions peuvent demander qqs calculs, donc à ne pas trop utiliser
	b2Vec2 GetReactionForce(float inv_dt) const;
	float GetReactionTorque(float inv_dt) const;

protected:
	// Etat
	bool mIsAlive;
	bool mToDestroy;

	// Type de joint
	JointType mType;

	// Objets physiques
	int mID;
	PhysicManager &mPhysicMgr;
	b2Joint *mJoint;

	// Propriétés cassable
	bool mIsBreakableMaxForce;
	ForceType mMaxForceType;
	float mMaxForce;
	b2Vec2 mMaxVecForce;
	bool mIsBreakableMaxTorque;
	float mMaxTorque;

	// Liste des joint à supprimer avant celui-ci
	std::list<int> mLinkedJointList;
};
