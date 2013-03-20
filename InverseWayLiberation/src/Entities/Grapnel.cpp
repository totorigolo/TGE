#include "Grapnel.h"
#include "EntityManager.h"
#include "../Tools/utils.h"

// Ctor & dtor
Grapnel::Grapnel(PhysicManager *mgr, int layer)
	: Entity(layer), mPhysicMgr(mgr), mJoint(nullptr), mJointID(-1)
{
	assert(mPhysicMgr && "n'est pas valide.");

	// Défini le type de l'Entity
	mType = EntityType::Grapnel;
}
Grapnel::~Grapnel()
{
	Destroy();
}

// Création du body
bool Grapnel::Create(std::shared_ptr<sf::Texture> textureHook, b2Body *bodyA, b2Vec2 ptA, b2Body *bodyB, b2Vec2 ptB)
{
	// On n'en crée pas de nouveau si il y en a déjà un
	if (mPhysicMgr->JointExists(mJointID))
		return false;

	// On vérifie les bodies et la texture
	assert(bodyA && bodyB && textureHook.get() && "ne sont pas valides.");

	// Change la texture du crochet
	mSpriteHook.setTexture(*textureHook);
	mSpriteHook.setOrigin(textureHook->getSize().x / 2.f, textureHook->getSize().y / 2.f);

	// Création du Joint
	mJoint = new DistanceJoint(mPhysicMgr, bodyA, ptA, bodyB, ptB);
	mJointID = mJoint->GetID();
	mIsAlive = true;
	mClock.restart();
	mIsRetracting = true;

	// Change les propriétés du Joint
	mJoint->SetBreakableByForce(true);
	mJoint->SetMaxForce(120.f);

	return true;
}

// Destruction du grappin
void Grapnel::Destroy()
{
	if (mPhysicMgr->JointExists(mJointID))
	{
		mPhysicMgr->DestroyJoint(mJointID);
		mJoint = nullptr;
		mJointID = -1;
	}
	mIsAlive = false;
}

// Mise à jour
void Grapnel::Update()
{
	// Si le Joint est valide
	if (mIsAlive && mJoint && mPhysicMgr->JointExists(mJointID))
	{
		// Mise à jour de la longueur
		if (mIsRetracting)
		{
			// Test de la force
			if (mJoint->GetReactionForce(1.f / mPhysicMgr->GetTimeStep()).Length() > 50.f)
				mIsRetracting = false;

			// Rétractation
			if (mIsRetracting)
			{
				float l = mJoint->GetLength() - (mClock.getElapsedTime().asSeconds());
				if (l > 0.f)
					mJoint->SetLength(l);
				else
					mIsRetracting = false;
				mClock.restart();
			}
		}

		// Mise à jour de la texture
		mSpriteHook.setPosition(b22sfVec(mJoint->GetAnchorB(), mPhysicMgr->GetPPM()));
		mSpriteHook.setRotation(- mJoint->GetBodyB()->GetAngle() * DPR);
	}
}

// Pour le rendu
void Grapnel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mPhysicMgr->JointExists(mJointID))
	{
		//target.draw(*mJoint);
		target.draw(mSpriteHook, states);
	}
}

// Accesseurs
Joint* Grapnel::GetJoint()
{
	return mJoint;
}
const Joint* Grapnel::GetJoint() const
{
	return mJoint;
}
PhysicManager* Grapnel::GetPhysicManager()
{
	return mPhysicMgr;
}
const PhysicManager* Grapnel::GetPhysicManager() const
{
	return mPhysicMgr;
}
