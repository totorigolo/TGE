#include "Player.h"
#include "EntityManager.h"
#include "../Tools/Error.h"
#include "../Game/InputManager.h"

// Ctor & dtor
Player::Player(b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer)
	: LivingBeing(position, texture, layer), mInputManager(InputManager::GetInstance())
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
		myAssert(mBody, "La b2Body n'est pas valide.");

		/* Traite les diff�rents mouvements (http://www.iforce2d.net/b2dtut/constant-speed) */
		// Calcule la vitesse max
		float maxVspeed = 4.f;
		float desiredVel = 0.f;

		// Obtient la vitesse actuelle
		b2Vec2 vel = mBody->GetLinearVelocity();

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

		// Applique le d�placement
		float velChange = desiredVel - vel.x;
		float impulse = mBody->GetMass() * velChange * 100;
		mBody->ApplyForceToCenter(b2Vec2(impulse, 0.f));

		// Saut
		if (mInputManager.KeyPressed(sf::Keyboard::Space))
		{
			if (mCanJump)
			{
				mBody->ApplyForceToCenter(b2Vec2(0.f, 370.f));
			}
		}
	}
}
