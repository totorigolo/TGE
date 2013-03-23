#include "PhysicManager.h"

//Ctor
PhysicManager::PhysicManager(b2Vec2 const& gravity, float ppm)
	: mWorld(gravity), mPPM(ppm), mTimeStep(1.f / 60.f), mLastJointID(0), mDebugDraw(this)
{
	// D�fini le ContactListener du monde
	mWorld.SetContactListener(&mContactListener);

	// D�fini le DebugDraw
	mWorld.SetDebugDraw(&mDebugDraw);
}

// Dtor
PhysicManager::~PhysicManager(void)
{
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
	assert(body && "est invalide.");

	// Supprime les joints attach�s
	b2JointEdge *je = body->GetJointList();
	while (je)
	{
		auto next = je->next;
		DestroyJoint(((Joint*) je->joint->GetUserData())->GetID());
		je = next;
	}

	// Supprime le b2Body
	mWorld.DestroyBody(body);
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
// Cr�ation / destruction
int PhysicManager::RegisterJoint(Joint *joint)
{
	mJointList[mLastJointID] = joint;
	joint->mID = mLastJointID;

	return mLastJointID++;
}
void PhysicManager::DestroyJoint(int jointID)
{
	// V�rifie que l'ID soit coh�rent
	assert(jointID >= 0 && "l'ID est impossible.");

	// V�rifie que la liste ne soit pas vide
	assert(mJointList.size() != 0 && "la liste des joints est vide.");

	// R�cup�re le joint
	auto itJoint = mJointList.find(jointID);
	assert(itJoint != mJointList.end() && "le joint n'existe pas.");
	Joint *joint = itJoint->second;

	// Supprime le joint
	mJointList.erase(itJoint);
	delete joint;
}
void PhysicManager::DestroyAllJoints()
{
	// D�truit les Joints
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		delete it->second;
		it = mJointList.erase(it);
	}
	mJointList.clear();

	assert(mWorld.GetJointCount() == 0 && "il ne devrait plus rester de joints.");
}
// b2Joint
b2Joint* PhysicManager::Createb2Joint(b2JointDef const* jointDef)
{
	return mWorld.CreateJoint(jointDef);
}
void PhysicManager::Destroyb2Joint(b2Joint *joint)
{
	assert(joint && "n'est pas valide.");
	mWorld.DestroyJoint(joint);
}
// Mide � jour
void PhysicManager::UpdateJoints()
{
	// Met � jour tous les joints
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		// Met � jour
		it->second->Update();

		// Supprime le joint si n�cessaire
		if (it->second->ToDestroy())
		{
			delete it->second;
			it = mJointList.erase(it);
		}

		// Sinon passe juste au suivant
		else
			++it;
	}
}
// Accesseurs
bool PhysicManager::JointExists(int jointID) const
{
	// V�rifie que l'ID soit coh�rent
	if (jointID < 0)
		return false;

	// V�rifie que la liste ne soit pas vide
	if (mJointList.size() == 0)
		return false;

	// R�cup�re le joint
	auto itJoint = mJointList.find(jointID);

	// V�rifie qu'il est valide
	if (itJoint != mJointList.end())
		if (itJoint->second->IsAlive())
			return true;
	return false;
}
Joint* PhysicManager::GetJoint(int jointID)
{
	// V�rifie qu'il est valide
	if (!JointExists(jointID))
		return nullptr;

	// Le retourne
	return mJointList.at(jointID);
}
const Joint* PhysicManager::GetJoint(int jointID) const
{
	// V�rifie qu'il est valide
	if (!JointExists(jointID))
		return nullptr;

	// Le retourne
	return mJointList.at(jointID);
}

// Suppression retard� (utile par ex dans le ContactListener)
/*void PhysicManager::ScheduleDestroyBody(b2Body *body)
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

	// On a r�ussi
	return true;
}*/

// Simulation
void PhysicManager::SetTimeStep(float timeStep)
{
	assert(timeStep > 0 && "un timeStep ne peut pas �tre n�gatif.");

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
	//DestroyScheduled();
}

// Gestion des propri�t�s du monde
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
// DebugDraw
void PhysicManager::DrawDebugData()
{
	mWorld.DrawDebugData();
	//mDebugDraw.DrawContacts();
}
void PhysicManager::SetDebugDrawTarget(sf::RenderTarget *target)
{
	mDebugDraw.SetRenderTarget(target);
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
unsigned int PhysicManager::GetJointCount()
{
	return mJointList.size();
}
std::map<int, Joint*>& PhysicManager::GetJointList()
{
	return mJointList;
}
const std::map<int, Joint*>& PhysicManager::GetJointList() const
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
