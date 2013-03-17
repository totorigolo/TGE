#pragma once
#include "ContactListener.h"
#include "Joint.h"
#include "../Entities/Entity.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <map>

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
	// Cr�ation / destruction
	int RegisterJoint(Joint *joint);
	void DestroyJoint(int jointID);
	void DestroyAllJoints();
	// b2Joint
	b2Joint* Createb2Joint(b2JointDef const* jointDef);
	void Destroyb2Joint(b2Joint *joint);
	// Mide � jour
	void UpdateJoints();
	// Accesseurs
	bool JointExists(int jointID) const;
	Joint* GetJoint(int jointID);
	const Joint* GetJoint(int jointID) const;

	// Suppression retard�e (utile par ex dans le ContactListener)
	// Ils sont supprim�s � la fin de Step() ou avec DestroyScheduled()
	/*void ScheduleDestroyBody(b2Body *body);
	void ScheduleDestroyb2Joint(b2Joint *joint);
	bool DestroyScheduled();*/

	// Simulation
	void SetTimeStep(float timeStep);
	float GetTimeStep() const;
	void Step(int velocityIt = 6, int positionIt = 2);

	// Gestion des propri�t�s du monde (gravit�)
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
	unsigned int GetJointCount();
	std::map<int, Joint*>& GetJointList();
	const std::map<int, Joint*>& GetJointList() const;

	// Obtient un StaticBody au hasard
	b2Body* GetAnyStaticBody();

private:
	// Pixels per meters
	float mPPM;

protected:
	// Propri�t�s
	float mTimeStep;

	// Monde Box2D
	b2World mWorld;

	// Contact Listener
	ContactListener mContactListener;

	// Liste des Joints
	int mLastJointID;
	std::map<int, Joint*> mJointList;

	// Suppression retard�e
	std::vector<b2Body*> mScheduledBodiesToDestroy;
	std::vector<b2Joint*> mScheduledJointsToDestroy;
};
