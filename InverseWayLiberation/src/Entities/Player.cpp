#include "Player.h"
#include "../Tools/utils.h"
#include "../Physics/Callback/AABBCallback.h"

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

/*/ Mise � jour
void Player::Update()
{
	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
	}
}*/

// Gestion des �v�nements
void Player::GetEvent(const PlayerEvent &playerEvent)
{
	// Si on est vivant
	if (mIsAlive && !mIsDead)
	{
		/* Cr�e une petite AABB sous le joueur * /
		b2AABB aabb;
		// Calcule les coordonn�es des bas gauche et droit
		b2Vec2 basDroite = mCollisionBody->GetBodyPosition() + (0.5f * mCollisionBody->GetBodySize());
		b2Vec2 basGauche = basDroite;
		// Haut � droite
		basDroite.x -= 0.005f;
		aabb.upperBound = basDroite;
		// Bas � gauche
		basGauche.x -= mCollisionBody->GetBodySize().x - 0.005f;
		basGauche.y -= 0.005f;
		aabb.lowerBound = basGauche;
		
		/* On v�rifie si on a qqchose sous nos pieds * /
		AABBCallback callback(false, mCollisionBody->GetBody()); // TODO: attribut
		mWorld->QueryAABB(&callback, aabb);
		//*/

		/* Traite les diff�rents mouvements */
		if (playerEvent == PlayerEvent::Left)
		{
			//if ((callback.GetFixture() && mCollisionBody->GetBody()->GetLinearVelocity().x >= -5.f)
			//	|| mCollisionBody->GetBody()->GetLinearVelocity().x >= -3.f)
			if ((mCanJump && mCollisionBody->GetLinearVelocity().x >= -5.f)
				|| mCollisionBody->GetLinearVelocity().x >= -3.f)
			{
				mCollisionBody->ApplyForceToCenter(b2Vec2(-5.f, 0.f));
			}
		}
		else if (playerEvent == PlayerEvent::Right)
		{
			if ((mCanJump && mCollisionBody->GetLinearVelocity().x <= 5.f)
				|| mCollisionBody->GetLinearVelocity().x <= 3.f)
			{
				mCollisionBody->ApplyForceToCenter(b2Vec2(5.f, 0));
			}
		}
		else if (playerEvent == PlayerEvent::Jump)
		{
			//if (callback.GetFixture())
			if (mCanJump)
			{
				mCollisionBody->ApplyForceToCenter(b2Vec2(0.f, 200.f)); // 300.f = un �tage
			}
		}
		else if (playerEvent == PlayerEvent::Crounch)
		{
		}
	}
}
