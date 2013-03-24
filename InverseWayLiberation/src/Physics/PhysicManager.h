#pragma once
#include "../Tools/NonCopyable.h"
#include "../Entities/Entity.h"
#include "ContactListener.h"
#include "DebugDraw.h"
#include <Box2D/Box2D.h>
#include <map>

class Joint;
class DebugDraw;
class PhysicManager : public NonCopyable
{
public:
	// Ctor & dtor
	PhysicManager(b2Vec2 const& gravity, float ppm = 30.f);
	virtual ~PhysicManager(void);
	
	// Gestion des body
	b2Body* CreateBody(b2BodyDef const* bodyDef);
	void DestroyBody(b2Body *body);
	void DestroyAllBody();
	void DestroyBodiesOut(const b2Vec2 &topleft, const b2Vec2 &bottomright);

	// Gestion des joints
	// Création / destruction
	int RegisterJoint(Joint *joint);
	void DestroyJoint(int jointID);
	void DestroyAllJoints();
	// b2Joint
	b2Joint* Createb2Joint(b2JointDef const* jointDef);
	void Destroyb2Joint(b2Joint *joint);
	// Mide à jour
	void UpdateJoints();
	// Accesseurs
	bool JointExists(int jointID) const;
	Joint* GetJoint(int jointID);
	const Joint* GetJoint(int jointID) const;

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

	// DebugDraw
	void DrawDebugData();
	void SetDebugDrawTarget(sf::RenderTarget *target);

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
	// Propriétés
	float mTimeStep;

	// Monde Box2D
	b2World mWorld;

	// Affichage debug
	DebugDraw mDebugDraw;

	// Contact Listener
	ContactListener mContactListener;

	// Liste des Joints
	int mLastJointID;
	std::map<int, Joint*> mJointList;
};
