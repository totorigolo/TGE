#include "Grapnel.h"
#include "../Tools/utils.h"

// Ctor & dtor
Grapnel::Grapnel(PhysicManager *mgr, int layer)
	: Entity(layer), mPhysicMgr(mgr), mJoint(nullptr), mJointIsCreated(false), mIsRetracting(false)
{
	assert(mPhysicMgr && "n'est pas valide.");

	// D�fini le type de l'Entity
	mType = EntityType::Grapnel;
}
Grapnel::~Grapnel()
{
	if (mJoint)
	{
		mJoint->Destroy();
		mJoint = nullptr;
	}
	mIsAlive = false;
}

// Cr�ation du body
bool Grapnel::Create(std::shared_ptr<sf::Texture> textureHook, b2Body *bodyA, b2Vec2 ptA, b2Body *bodyB, b2Vec2 ptB)
{
	// On n'en cr�e pas de nouveau si il y en a d�j� un
	if (mJointIsCreated)
		return false;

	// On v�rifie les bodies et la texture
	if (!bodyA || !bodyB || !textureHook.get())
		return false;

	// Change la texture du crochet
	mSpriteHook.setTexture(*textureHook);
	mSpriteHook.setOrigin(textureHook->getSize().x / 2.f, textureHook->getSize().y / 2.f);

	// Cr�ation du Joint
	mJoint = new DistanceJoint(mPhysicMgr, bodyA, ptA, bodyB, ptB);
	mJointIsCreated = true;
	mIsAlive = true;
	mClock.restart();
	mIsRetracting = true;

	// Change les propri�t�s du Joint
	mJoint->SetBreakableByForce(true);
	mJoint->SetMaxForce(600.f);

	return true;
}

// Destruction du grappin
void Grapnel::Destroy()
{
	delete mJoint;
	mJointIsCreated = false;
	mIsAlive = false;
}

// Mise � jour
void Grapnel::Update()
{
	// Si le Joint est valide
	if (mIsAlive && mJoint && mJointIsCreated)
	{
		// Mise � jour de la longueur
		if (mIsRetracting)
		{
			// Test de la force
			if (mJoint->GetReactionForce(1.f / mPhysicMgr->GetTimeStep()).Length() > 50.f)
				mIsRetracting = false;

			// R�tractation
			if (mIsRetracting)
			{
				float l = mJoint->GetLength() - mClock.getElapsedTime().asSeconds();
				if (l > 0.f)
					mJoint->SetLength(l);
				else
					mIsRetracting = false;
				mClock.restart();
			}
		}

		// Mise � jour du joint
		mJoint->Update();
		if (mJoint->IsNull()) // Si il a cass�
		{
			Destroy();
			return;
		}

		// Mise � jour de la texture
		mSpriteHook.setPosition(b22sfVec(mJoint->GetAnchorB(), mPhysicMgr->GetPPM()));
		mSpriteHook.setRotation(- mJoint->GetBodyB()->GetAngle() * DPR);
	}
}

// Pour le rendu
void Grapnel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mJointIsCreated)
	{
		target.draw(*mJoint);
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
