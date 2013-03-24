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

	// Demande l'espionnage de touches
	mInputManager.AddSpyedKey(sf::Keyboard::Space);
}
Player::~Player()
{
	mIsAlive = false;

	// Arr�te l'espionnage de touches
	mInputManager.RemoveSpyedKey(sf::Keyboard::Space);
}

// Mise � jour
void Player::Update()
{
	// Appel de la mise � jour de LivingBeing
	LivingBeing::Update();

	// Met � jour suivant les �v�nements
	UpdateEvents();
}

// Gestion des �v�nements
void Player::UpdateEvents()
{
	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
		assert(mBody && "n'est pas valide.");

		/* Traite les diff�rents mouvements */
		bool moved = false;
		// Gauche
		if (mInputManager.GetKeyState(sf::Keyboard::Q))
		{
			if ((mCanJump && mBody->GetLinearVelocity().x >= -5.f)
				|| mBody->GetLinearVelocity().x >= -3.f)
			{
				mBody->ApplyForceToCenter(b2Vec2(-6.f, 0.f));
				moved = true;
			}
		}

		// Droite
		if (mInputManager.GetKeyState(sf::Keyboard::D))
		{
			if ((mCanJump && mBody->GetLinearVelocity().x <= 5.f)
				|| mBody->GetLinearVelocity().x <= 3.f)
			{
				mBody->ApplyForceToCenter(b2Vec2(6.f, 0));
				moved = true;
			}
		}

		// Saut
		if (mInputManager.KeyPressed(sf::Keyboard::Space))
		{
			if (mCanJump)
			{
				mBody->ApplyForceToCenter(b2Vec2(0.f, 260.f)); // 300.f = un �tage
			}
		}

		// On �vite que le Player soit une savonette
		if (!moved)
		{
			mBody->ApplyForceToCenter(- 2.f * mBody->GetLinearVelocity());
		}
	}
}
