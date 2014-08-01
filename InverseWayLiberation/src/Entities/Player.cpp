#include "stdafx.h"
#include "Player.h"
#include "../Resources/ResourceManager.h"

// Ctor
Player::Player()
	: Hum(0), mInputManager(InputManager::GetInstance()), mJumpKey(sf::Keyboard::Space)
{
	// D�fini le type de l'Entity
	mType = EntityType::Player;
}

// Dtor
Player::~Player()
{
}

// Mise � jour
void Player::Update()
{
	Hum::Update();

	// Met � jour les �v�nements
	UpdateEvents();
}

// Gestion des �v�nements
void Player::UpdateEvents()
{
	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
		float strenghFactor = 0.5f + ((static_cast<float>(GetStrengh()) + 20.f) / 40.f) / 2.f;

		/* Traite les diff�rents mouvements (http://www.iforce2d.net/b2dtut/constant-speed) */
		// Calcule la vitesse max
		float maxVspeed = 4.f;
		float desiredVel = 0.f;

		// Obtient la vitesse actuelle
		b2Vec2 vel = GetBody()->GetLinearVelocity();

		bool moved = false;
		// Gauche
		if (mInputManager.IsKeyPressed(sf::Keyboard::Q))
		{
			desiredVel = b2Max(vel.x - 0.1f, -maxVspeed);
			moved = true;
		}
		// Droite
		if (mInputManager.IsKeyPressed(sf::Keyboard::D))
		{
			desiredVel = b2Min(vel.x + 0.1f, maxVspeed);
			moved = true;
		}
		// On �vite que le Player soit une savonette
		if (!moved)
		{
			desiredVel = vel.x * 0.8f;
		}

		// D�placement pour le Hull
		if (moved) mHasMoved = true;

		// Applique le d�placement
		float velChange = desiredVel - vel.x;
		float impulse = GetBody()->GetMass() * velChange * 100.f;
		GetBody()->ApplyForceToCenter(b2Vec2(impulse, 0.f), true);

		// Saut
		if (mInputManager.KeyPressed(sf::Keyboard::Space))
		{
			if (mCanJump)
			{
				float impulse = GetBody()->GetMass() * 6.3f * strenghFactor;
				GetBody()->ApplyLinearImpulse(b2Vec2(0, impulse), GetBody()->GetWorldCenter(), true);
				mCanJump = false;
			}
		}
	}
}