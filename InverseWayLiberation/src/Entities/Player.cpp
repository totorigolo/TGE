#include "Player.h"
#include "EntityManager.h"
#include "../Tools/utils.h"
#include "../Physics/Callback/AABBCallback.h"
#include <cassert>

// Ctor & dtor
Player::Player(PhysicManager *physicMgr, b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer)
	: LivingBeing(physicMgr, position, texture, layer), mInputManager(InputManager::GetInstance())
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

	// Met à jour suivant les évènements
	UpdateEvents();
}

// Gestion des évènements
void Player::UpdateEvents()
{
	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
		assert(mBody && "n'est pas valide.");

		/* Traite les différents mouvements */
		if (mInputManager.GetKeyState(sf::Keyboard::Q))
		{
			if ((mCanJump && mBody->GetLinearVelocity().x >= -5.f)
				|| mBody->GetLinearVelocity().x >= -3.f)
			{
				mBody->ApplyForceToCenter(b2Vec2(-5.f, 0.f));
			}
		}
		else if (mInputManager.GetKeyState(sf::Keyboard::D))
		{
			if ((mCanJump && mBody->GetLinearVelocity().x <= 5.f)
				|| mBody->GetLinearVelocity().x <= 3.f)
			{
				mBody->ApplyForceToCenter(b2Vec2(5.f, 0));
			}
		}
		else if (mInputManager.GetKeyState(sf::Keyboard::Space))
		{
			if (mCanJump)
			{
				mBody->ApplyForceToCenter(b2Vec2(0.f, 260.f)); // 300.f = un étage
			}
		}
		else if (mInputManager.GetKeyState(sf::Keyboard::LControl))
		{
		}
	}
}
