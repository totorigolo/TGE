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
	
	// Gestion des body (Seuls les Bodies devraient appeler ces fonctions)
	void RegisterBody(Body *body);
	b2Body* CreateBody(b2BodyDef const* bodyDef, Body *body);
	void DestroyBody(Body *body);
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
	
	void SetPPM(float ppm) { mPPM = ppm; }
	void SetMPP(float mmp) { mPPM = 1.f / mmp; }
	inline float GetPPM() const { return mPPM; }
	inline float GetMPP() const { return 1.f / mPPM; }

	Body* GetAnyStaticBody();

private:
	float mPPM; // Pixels per meters

protected:
	// Listes d'objets
	std::list<Body*> mBodyList;
	std::list<Joint*> mJointList;

	// Contacts
	ContactListener mContactListener;
};
