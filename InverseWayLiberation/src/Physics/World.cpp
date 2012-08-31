#include "World.h"
#include <iostream> // Pour le message de GetAnyStaticBody()

//Ctor
World::World(b2Vec2 const& gravity, float ppm)
	: b2World(gravity), mPPM(ppm)
{
	SetContactListener(&mContactListener);
}

// Dtor
World::~World(void)
{
	DestroyAllJoints();
	DestroyAllBody();
}

// Gestion des body
void World::RegisterBody(Body *body)
{
	mBodyList.push_back(body);
}
b2Body* World::CreateBody(b2BodyDef const* bodyDef, Body *body)
{
	b2Body *b = b2World::CreateBody(bodyDef);
	body->SetBody(b);
	b->SetUserData(body);
	return b;
}
void World::DestroyBody(Body *body, bool _delete, bool remove)
{
	if (body)
	{
		body->DestroyAllJoints();
		if (body->GetBody())
			b2World::DestroyBody(body->GetBody());
		body->SetBody(nullptr);
		if (remove)
			mBodyList.remove(body);
		if (_delete)
			delete body;
	}
}
void World::DestroyAllBody()
{
	for (auto it = mBodyList.begin(); it != mBodyList.end(); )
	{
		auto it2 = it; ++it2;
		this->DestroyBody(*it, true, false);
		it = it2;
	}
	// Au cas où...
	mBodyList.clear();
}

// Gestion des joints
void World::RegisterJoint(Joint *joint)
{
	mJointList.push_back(joint);
}
b2Joint* World::CreateJoint(b2JointDef const* jointDef, Joint *joint)
{
	b2Joint *j = b2World::CreateJoint(jointDef);
	j->SetUserData(joint);
	joint->SetJoint(j);
	return j;
}
void World::DestroyJoint(Joint *joint, bool _delete, bool remove)
{
	if (joint)
	{
		if (joint->GetJoint())
			b2World::DestroyJoint(joint->GetJoint());
		joint->SetJoint(nullptr);
		if (remove)
			mJointList.remove(joint);
		if (_delete)
			delete joint;
	}
}
void World::DestroyAllJoints()
{
	for (auto it = mJointList.begin(); it != mJointList.end(); )
	{
		auto it2 = it; ++it2;
		this->DestroyJoint(*it);
		it = it2;
	}
	// Au cas où...
	mJointList.clear();
}

// Accesseurs
Body* World::GetAnyStaticBody() // Pour le mouseJoint
{
	// Cherche le premier static body existant
	for (auto it = mBodyList.begin(); it != mBodyList.end(); ++it)
	{
		if ((*it)->GetBody()->GetType() == b2_staticBody)
		{
			return *it;
		}
	}

	// Sinon crée le static body
	std::cerr << "Warning !! Untextured static body created for MouseJoint !" << std::endl;

	/* Crée le body */
	Body *body = new Body(this);
	// BodyDef
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	b2Body *b2body = this->CreateBody(&bodyDef, body);
	// Fixture
	b2FixtureDef fixtureDef;
	b2body->CreateFixture(&fixtureDef);

	b2body->SetUserData(body);
	
	mBodyList.push_back(body);
	return body;
}
