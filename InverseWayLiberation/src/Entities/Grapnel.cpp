#include "Grapnel.h"
#include "../Tools/utils.h"
#include "../Tools/Error.h"
#include "../Physics/PhysicManager.h"
#include "../Physics/Joints/DistanceJoint.h"

// Ctor & dtor
Grapnel::Grapnel(int layer, unsigned int ID)
	: Entity(layer, ID), mJoint(nullptr), mJointID(-1),
	mPhysicMgr(PhysicManager::GetInstance())
{
	// D�fini le type de l'Entity
	mType = EntityType::Grapnel;
}
Grapnel::~Grapnel()
{
	Destroy();
}

// Cr�ation du body
bool Grapnel::Create(const std::shared_ptr<Texture> &textureHook, b2Body *bodyA, b2Vec2 ptA, b2Body *bodyB, b2Vec2 ptB)
{
	// On n'en cr�e pas de nouveau si il y en a d�j� un
	if (mPhysicMgr.JointExists(mJointID))
		return false;

	// On v�rifie les bodies et la texture
	myAssert(bodyA, "Le b2Body n'est pas valide !");
	myAssert(bodyB, "Le b2Body n'est pas valide !");

	// Change la texture du crochet
	mTexture = textureHook;
	myAssert(mTexture.get(), "La texture n'est pas charg�e.");
	mSpriteHook.setTexture(*mTexture);
	mSpriteHook.setOrigin(mTexture->getSize().x / 2.f, mTexture->getSize().y / 2.f);

	// Cr�ation du Joint
	DistanceJointDef def(bodyA, ptA, bodyB, ptB);
	mJoint = new DistanceJoint(def);
	mJointID = mJoint->GetID();
	mIsAlive = true;
	mClock.restart();
	mIsRetracting = true;

	// Change les propri�t�s du Joint
	mJoint->SetBreakableByForce(true);
	mJoint->SetMaxForce(120.f);

	return true;
}

// Destruction du grappin
void Grapnel::Destroy()
{
	if (mPhysicMgr.JointExists(mJointID))
	{
		mPhysicMgr.DestroyJoint(mJointID);
		mJoint = nullptr;
		mJointID = -1;
	}
	mIsAlive = false;
	mTexture.reset();
}

// Mise � jour
void Grapnel::Update()
{
	// Si le Joint est valide
	if (mIsAlive && mJoint && mPhysicMgr.JointExists(mJointID))
	{
		// Mise � jour de la longueur
		if (mIsRetracting)
		{
			// Test de la force
			if (mJoint->GetReactionForce(1.f / mPhysicMgr.GetTimeStep()).Length() > 50.f)
				mIsRetracting = false;

			// R�tractation
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

		// Mise � jour de la texture
		mSpriteHook.setPosition(b22sfVec(mJoint->GetRelativeAnchorB(), mPhysicMgr.GetPPM()));
		mSpriteHook.setRotation(- mJoint->GetBodyB()->GetAngle() * DPR);
	}
}

// Pour le rendu
void Grapnel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mPhysicMgr.JointExists(mJointID))
	{
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