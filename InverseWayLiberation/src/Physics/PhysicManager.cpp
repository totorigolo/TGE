#include "PhysicManager.h"

//Ctor
PhysicManager::PhysicManager(b2Vec2 const& gravity, float ppm)
	: mWorld(gravity), mPPM(ppm), mTimeStep(1.f / 60.f)
{
	// Défini le ContactListener du monde
	mWorld.SetContactListener(&mContactListener);
}

// Dtor
PhysicManager::~PhysicManager(void)
{
	// Au cas où
	DestroyAllJoints();
	DestroyAllBody();
}

// Gestion des body
b2Body* PhysicManager::CreateBody(b2BodyDef const* bodyDef)
{
	return mWorld.CreateBody(bodyDef);
}
void PhysicManager::DestroyBody(b2Body *body)
{
	if (body)
	{
		mWorld.DestroyBody(body);
	}
}
void PhysicManager::DestroyAllBody()
{
	b2Body *b = mWorld.GetBodyList(), *bb = nullptr;

	while (b)
	{
		bb = b;
		b = b->GetNext();

		DestroyBody(bb);
	}
}

// Gestion des joints
void PhysicManager::DestroyAllJoints()
{
	// Détruit les Joints
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		auto it2 = it; ++it2;
		this->DestroyJoint(*it);
		it = it2;
	}
	mJointList.clear();

	// Supprime les b2Joints
	b2Joint *j = mWorld.GetJointList(), *jj = nullptr;
	while (j)
	{
		jj = j;
		j = j->GetNext();

		Destroyb2Joint(jj);
	}
}
// b2Joint
b2Joint* PhysicManager::Createb2Joint(b2JointDef const* jointDef)
{
	return mWorld.CreateJoint(jointDef);
}
void PhysicManager::Destroyb2Joint(b2Joint *joint)
{
	if (joint)
	{
		mWorld.DestroyJoint(joint);
	}
}
// Joint
void PhysicManager::RegisterJoint(Joint *joint)
{
	mJointList.push_back(joint);
}
b2Joint* PhysicManager::CreateJoint(b2JointDef const* jointDef, Joint *joint)
{
	b2Joint *j = Createb2Joint(jointDef);
	j->SetUserData(joint);
	joint->SetJoint(j);
	return j;
}
void PhysicManager::DestroyJoint(Joint *joint, bool _delete, bool remove)
{
	if (joint)
	{
		if (joint->GetJoint())
			Destroyb2Joint(joint->GetJoint());
		joint->SetJoint(nullptr);
		if (remove)
			mJointList.remove(joint);
		if (_delete)
			delete joint;
	}
}

// Suppression retardé (utile par ex dans le ContactListener)
void PhysicManager::ScheduleDestroyBody(b2Body *body)
{
	mScheduledBodiesToDestroy.push_back(body);
}
void PhysicManager::ScheduleDestroyb2Joint(b2Joint *joint)
{
	mScheduledJointsToDestroy.push_back(joint);
}
bool PhysicManager::DestroyScheduled()
{
	// Si le monde est en train de simuler (dans un Step()), on annule
	if (mWorld.IsLocked())
		return false;

	// Supprime les bodies
	for (unsigned int i = 0U; i < mScheduledBodiesToDestroy.size(); ++i)
	{
		DestroyBody(mScheduledBodiesToDestroy[i]);
	}
	mScheduledBodiesToDestroy.clear();
	
	// Supprime les joints
	for (unsigned int i = 0U; i < mScheduledJointsToDestroy.size(); ++i)
	{
		Destroyb2Joint(mScheduledJointsToDestroy[i]);
	}
	mScheduledJointsToDestroy.clear();

	// On a réussi
	return true;
}

// Simulation
void PhysicManager::SetTimeStep(float timeStep)
{
	assert(timeStep > 0 && "un timeStep ne peut pas être négatif.");

	mTimeStep = mTimeStep;
}
float PhysicManager::GetTimeStep() const
{
	return mTimeStep;
}
void PhysicManager::Step(int velocityIt, int positionIt)
{
	// TODO: Fixer le Timestep
	mWorld.Step(mTimeStep, velocityIt, positionIt);
	DestroyScheduled();
}

// Gestion des propriétés du monde
void PhysicManager::SetGravity(const b2Vec2 &gravity)
{
	mWorld.SetGravity(gravity);
}
b2Vec2 PhysicManager::GetGravity()
{
	return mWorld.GetGravity();
}

/* Accesseurs */
// Le monde
b2World* PhysicManager::GetWorld()
{
	return &mWorld;
}
const b2World* PhysicManager::GetWorld() const
{
	return &mWorld;
}
// La liste des Bodies
int PhysicManager::GetBodyCount()
{
	return mWorld.GetBodyCount();
}
b2Body* PhysicManager::GetBodyList()
{
	return mWorld.GetBodyList();
}
const b2Body* PhysicManager::GetBodyList() const
{
	return mWorld.GetBodyList();
}
// La liste des Joints
// b2Joints
unsigned int PhysicManager::Getb2JointCount()
{
	return mWorld.GetJointCount();
}
b2Joint* PhysicManager::Getb2JointList()
{
	return mWorld.GetJointList();
}
const b2Joint* PhysicManager::Getb2JointList() const
{
	return mWorld.GetJointList();
}
// Joints
unsigned int PhysicManager::GetJointCount()
{
	return mJointList.size();
}
std::list<Joint*>& PhysicManager::GetJointList()
{
	return mJointList;
}
const std::list<Joint*>& PhysicManager::GetJointList() const
{
	return mJointList;
}

// Obtient un StaticBody au hasard
b2Body* PhysicManager::GetAnyStaticBody()
{
	b2Body *b = mWorld.GetBodyList();

	while (b)
	{
		if (b->GetType() == b2_staticBody)
		{
			return b;
		}

		b = b->GetNext();
	}

	// Il n'y a pas de StaticBody
	return nullptr;
}
