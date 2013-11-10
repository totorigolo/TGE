#include "PhysicManager.h"
#include "Joint.h"
#include "DebugDraw.h"
#include "ContactListener.h"
#include "../Tools/Error.h"
#include "../Tools/Parser.h"
#include "../Entities/Entity.h"
#include "../Entities/EntityManager.h"

//Ctor
PhysicManager::PhysicManager()
	: // Valeurs par d�faut
	mWorld(b2Vec2(0.f, 0.f)),
	mPPM(100.f),
	mTimeStep(1.f / 60.f),
	lastBasicBodiesID(0U),
	// Autres
	mLastJointID(0),
	mDebugDraw(*this)
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
	myAssert(body, "Le b2Body est invalide.");

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
void PhysicManager::DestroyBodiesOut(const b2Vec2 &topleft, const b2Vec2 &bottomright)
{
	// Destruction des bodies en dehors de la zone
	b2Body *b = GetBodyList(), *bb = nullptr;
	while (b)
	{
		// On supprime seulement les dynamicBodies
		if (b->GetType() == b2_dynamicBody)
		{
			// V�rifie si l'objet est hors du monde
			b2Vec2 pos = b->GetPosition();
			if (pos.x < topleft.y || pos.x > bottomright.y || pos.y < bottomright.x || pos.y > bottomright.y)
			{
				bb = b;
				b = b->GetNext();
				EntityManager::GetInstance().DestroyEntity((Entity*) bb->GetUserData());
			}

			// Sinon passe simplement au suivant
			else
				b = b->GetNext();
		}
		// Le body n'est pas un dynamicBody
		else
			b = b->GetNext();
	}
}

// Gestion des joints
// Cr�ation / destruction
int PhysicManager::RegisterJoint(Joint *joint)
{
	mJointList[mLastJointID] = std::unique_ptr<Joint>(joint);
	joint->mID = mLastJointID;

	return mLastJointID++;
}
void PhysicManager::DestroyJoint(int jointID)
{
	// V�rifie que l'ID soit coh�rent
	myAssert(jointID >= 0, "L'ID \""+ Parser::intToString(jointID) +"\" est impossible (< 0).");

	// V�rifie que la liste ne soit pas vide
	myAssert(mJointList.size() != 0, "La liste des joints est vide.");

	// R�cup�re le joint
	auto itJoint = mJointList.find(jointID);
	myAssert(itJoint != mJointList.end(), "Le joint #\""+ Parser::intToString(jointID) +"\" n'existe pas.");

	// Supprime le joint
	mJointList.erase(itJoint);
}
void PhysicManager::DestroyAllJoints()
{
	// D�truit les Joints
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		it = mJointList.erase(it);
	}
	mJointList.clear();

	myAssert(mWorld.GetJointCount() == 0, "Il ne devrait plus rester de joints.");
}
// b2Joint
b2Joint* PhysicManager::Createb2Joint(b2JointDef const* jointDef)
{
	return mWorld.CreateJoint(jointDef);
}
void PhysicManager::Destroyb2Joint(b2Joint *joint)
{
	myAssert(joint, " Le joint n'est pas valide.");
	mWorld.DestroyJoint(joint);
}
// Mise � jour
void PhysicManager::UpdateJoints()
{
	// Met � jour tous les joints
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		// Met � jour
		it->second->Update();

		// Supprime le joint si n�cessaire
		if (it->second->ToDestroy())
			it = mJointList.erase(it);

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
	return mJointList.at(jointID).get();
}
const Joint* PhysicManager::GetJoint(int jointID) const
{
	// V�rifie qu'il est valide
	if (!JointExists(jointID))
		return nullptr;

	// Le retourne
	return mJointList.at(jointID).get();
}

// Simulation
void PhysicManager::SetTimeStep(float timeStep)
{
	myAssert(timeStep > 0, "Un timeStep ne peut pas �tre n�gatif ("+ Parser::floatToString(timeStep) +").");

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
}

// Gestion des propri�t�s du monde
void PhysicManager::SetGravity(const b2Vec2 &gravity)
{
	mWorld.SetGravity(gravity);
}
b2Vec2 PhysicManager::GetGravity() const
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
std::map<int, std::unique_ptr<Joint>>& PhysicManager::GetJointList()
{
	return mJointList;
}
const std::map<int, std::unique_ptr<Joint>>& PhysicManager::GetJointList() const
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
