#include "Player.h"
#include "EntityManager.h"
#include "../Tools/utils.h"
#include "../Physics/Callback/AABBCallback.h"
#include <iostream>

// Ctor & dtor
Player::Player(PhysicManager *physicMgr, b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer)
	: LivingBeing(physicMgr, position, texture, layer)
{
	// Change le type
	mType = EntityType::Player;

	// Tout est Ok
	mIsAlive = true;
	mIsDead = false;
}
Player::~Player()
{
	mIsAlive = false;
}

// Mise � jour
void Player::Update()
{
	// Appel de la mise � jour de LivingBeing
	LivingBeing::Update();

	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
	}
}

// Gestion des �v�nements
void Player::GetEvent(const MovementEvent &playerEvent)
{
	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
		/* Traite les diff�rents mouvements */
		if (playerEvent == Left)
		{
			if ((mCanJump && mBody->GetLinearVelocity().x >= -5.f)
				|| mBody->GetLinearVelocity().x >= -3.f)
			{
				mBody->ApplyForceToCenter(b2Vec2(-5.f, 0.f));
			}
		}
		else if (playerEvent == Right)
		{
			if ((mCanJump && mBody->GetLinearVelocity().x <= 5.f)
				|| mBody->GetLinearVelocity().x <= 3.f)
			{
				mBody->ApplyForceToCenter(b2Vec2(5.f, 0));
			}
		}
		else if (playerEvent == Jump)
		{
			if (mCanJump)
			{
				mBody->ApplyForceToCenter(b2Vec2(0.f, 260.f)); // 300.f = un �tage
			}
		}
		else if (playerEvent == Crounch)
		{
		}
	}
}

// Gestion des d�pendences
void Player::DependencyDestroyed(void *dependency)
{
	// Appel de la fonction de LivingBeing
	LivingBeing::DependencyDestroyed(dependency);

	// Voir avec l'h�ritage de LivingBeing, qui est appel� ?
	std::cout << "Player::DependencyDestroyed()" << std::endl;

	/*/ V�rifie que le Body ne soit pas supprim�
	if (dependency == mCollisionBody)
	{
		mIsAlive = false;
		mCollisionBody = nullptr;
		EntityManager::GetInstance().DestroyEntity(this, false);
		delete this;
	}*/
}
