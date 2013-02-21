#pragma once
#include <Box2D/Box2D.h>
#include <vector>
#include "ContactListener.h"
#include "Joint.h"

class Joint;
class PhysicManager
{
public:
	// Ctor & dtor
	PhysicManager(b2Vec2 const& gravity, float ppm = 30.f);
	virtual ~PhysicManager(void);
	
	// Gestion des body
	b2Body* CreateBody(b2BodyDef const* bodyDef);
	void DestroyBody(b2Body *body);
	void DestroyAllBody();

	// Gestion des joints
	void DestroyAllJoints();
	// b2Joint
	b2Joint* Createb2Joint(b2JointDef const* jointDef);
	void Destroyb2Joint(b2Joint *joint);
	// Joint
	void RegisterJoint(Joint *joint);
	b2Joint* CreateJoint(b2JointDef const* jointDef, Joint *joint);
	void DestroyJoint(Joint *joint, bool _delete = true, bool remove = true);

	// Suppression retardée (utile par ex dans le ContactListener)
	// Ils sont supprimés à la fin de Step() ou avec DestroyScheduled()
	void ScheduleDestroyBody(b2Body *body);
	void ScheduleDestroyb2Joint(b2Joint *joint);
	bool DestroyScheduled();

	// Simulation
	void SetTimeStep(float timeStep);
	float GetTimeStep() const;
	void Step(int velocityIt = 6, int positionIt = 2);

	// Gestion des propriétés du monde (gravité)
	void SetGravity(const b2Vec2 &gravity);
	b2Vec2 GetGravity();

	// Gestion des pixels / metre
	void SetPPM(float ppm) { mPPM = ppm; }
	void SetMPP(float mmp) { mPPM = 1.f / mmp; }
	inline float GetPPM() const { return mPPM; }
	inline float GetMPP() const { return 1.f / mPPM; }

	/* Accesseurs */
	// Le monde
	b2World* GetWorld();
	const b2World* GetWorld() const;

	// La liste des Bodies
	int GetBodyCount();
	b2Body* GetBodyList();
	const b2Body* GetBodyList() const;

	// La liste des Joints
	// b2Joints
	unsigned int Getb2JointCount();
	b2Joint* Getb2JointList();
	const b2Joint* Getb2JointList() const;
	// Joints
	unsigned int GetJointCount();
	std::list<Joint*>& GetJointList();
	const std::list<Joint*>& GetJointList() const;

	// Obtient un StaticBody au hasard
	b2Body* GetAnyStaticBody(); // TODO: const ?

private:
	// Pixels per meters
	float mPPM;

protected:
	// Propriétés
	float mTimeStep;

	// Monde Box2D
	b2World mWorld;

	// Contact Listener
	ContactListener mContactListener;

	// Liste des Joints
	std::list<Joint*> mJointList;

	// Suppression retardée
	std::vector<b2Body*> mScheduledBodiesToDestroy;
	std::vector<b2Joint*> mScheduledJointsToDestroy;
};
