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

// Mise à jour
void Player::Update()
{
	// Appel de la mise à jour de LivingBeing
	LivingBeing::Update();

	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
	}
}

// Gestion des évènements
void Player::GetEvent(const MovementEvent &playerEvent)
{
	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
		/* Traite les différents mouvements */
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
				mBody->ApplyForceToCenter(b2Vec2(0.f, 260.f)); // 300.f = un étage
			}
		}
		else if (playerEvent == Crounch)
		{
		}
	}
}

// Gestion des dépendences
void Player::DependencyDestroyed(void *dependency)
{
	// Appel de la fonction de LivingBeing
	LivingBeing::DependencyDestroyed(dependency);

	// Voir avec l'héritage de LivingBeing, qui est appelé ?
	std::cout << "Player::DependencyDestroyed()" << std::endl;

	/*/ Vérifie que le Body ne soit pas supprimé
	if (dependency == mCollisionBody)
	{
		mIsAlive = false;
		mCollisionBody = nullptr;
		EntityManager::GetInstance().DestroyEntity(this, false);
		delete this;
	}*/
}
