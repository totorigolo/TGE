#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include <list>
#include "Bodies/Body.h"
#include "Joints/Joint.h"
#include "ContactListener.h"

class Body;
class Joint;
class World : public b2World
{
public:
	// Ctor & dtor
	World(b2Vec2 const& gravity, float ppm = 30.f);
	virtual ~World(void);
	
	// Gestion des body
	void RegisterBody(Body *body);
	b2Body* CreateBody(b2BodyDef const* bodyDef, Body *body);
	void DestroyBody(Body *body, bool _delete = true, bool remove = true);
	void DestroyAllBody();

	// Gestion des joints
	void RegisterJoint(Joint *joint);
	b2Joint* CreateJoint(b2JointDef const* jointDef, Joint *joint);
	void DestroyJoint(Joint *joint, bool _delete = true, bool remove = true);
	void DestroyAllJoints();

	// Accesseurs
	std::list<Body*> & GetBodyList() { return mBodyList; }
	std::list<Joint*> & GetJointList() { return mJointList; }
	std::list<Body*> const& GetBodyList() const { return mBodyList; }
	std::list<Joint*> const& GetJointList() const { return mJointList; }

	Body* GetAnyStaticBody(); // Pour le mouseJoint
	
	void SetPPM(float ppm) { mPPM = ppm; }
	void SetMPP(float mmp) { mPPM = 1.f / mmp; }
	float GetPPM() const { return mPPM; }
	float GetMPP() const { return 1.f / mPPM; }

private:
	float mPPM; // Pixels per meters

protected:
	// Listes d'objets
	std::list<Body*> mBodyList;
	std::list<Joint*> mJointList;

	// Contacts
	ContactListener mContactListener;
};
