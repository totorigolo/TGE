#pragma once
#include "DebugDraw.h"
#include "ContactListener.h"

class Joint;
class Entity;
class PhysicManager : public Singleton<PhysicManager>
{
protected:
	friend class Singleton<PhysicManager>;

	// Ctor & dtor
	PhysicManager();
	virtual ~PhysicManager(void);
	
public:
	// Gestion des body
	b2Body* CreateBody(b2BodyDef const* bodyDef);
	void DestroyBody(b2Body *body);
	void DestroyAllBody();
	void DestroyBodiesOut(const b2Vec2 &topleft, const b2Vec2 &bottomright, b2Body *skip = nullptr);

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
	// Gestion des noms
	bool Name(const std::string &name, Joint *joint); // true if already exists
	void Anonymize(const std::string &name);
	std::string GetName(Joint *const joint) const;
	// Accesseurs
	bool JointExists(int jointID) const;
	Joint* GetJoint(int jointID);
	Joint* GetJoint(const std::string &name);
	const Joint* GetJoint(int jointID) const;
	const Joint* GetJoint(const std::string &name) const;

	// Simulation
	void SetTimeStep(float timeStep);
	float GetTimeStep() const;
	void Step(int velocityIt = 6, int positionIt = 2);

	// Gestion des propriétés du monde (gravité)
	void SetGravity(const b2Vec2 &gravity);
	b2Vec2 GetGravity() const;

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
	std::map<int, std::unique_ptr<Joint>>& GetJointList();
	const std::map<int, std::unique_ptr<Joint>>& GetJointList() const;

	// Obtient un StaticBody au hasard
	b2Body* GetAnyStaticBody();

private:
	// Pixels per meters
	float mPPM;

protected:
	// Variables pour le Step
	float mTimeStep;
	sf::Clock mStepClock;

	// Monde Box2D
	b2World mWorld;

	// Affichage debug
	DebugDraw mDebugDraw;

	// Contact Listener
	ContactListener mContactListener;

	// Gestion des IDs des BasicBodies
	unsigned int lastBasicBodiesID;

	// Liste des Joints
	int mLastJointID;
	std::map<int, std::unique_ptr<Joint>> mJointList; // <id, unique_ptr<joint>>

	// Liste des noms des joints
	typedef boost::bimap<std::string, Joint*> name_bimap;
	typedef name_bimap::value_type name_bimap_position;
	name_bimap mNames;
};
