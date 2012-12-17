#include "Entity.h"
#include "../../Tools/utils.h"
#include <cassert>
#include <iostream>

// Fonction d'animation
void testAnimation(sf::Sprite& animated, float progress)
{
	animated.setTexture(*ResourceManager::GetInstance().GetTextureMap()["skyrim"]);
	//animated.setOrigin(u2f(animated.getTexture()->getSize()) / 2.f);
	int x = progress * static_cast<float>(animated.getTexture()->getSize().x);
	int y = progress * static_cast<float>(animated.getTexture()->getSize().y);
	animated.setTextureRect(sf::IntRect(x, y, 40, 150));
	animated.setOrigin(0.f, 0.f);
}

// Ctor & dtor
Entity::Entity(World *world, sf::Texture *texture, b2Vec3 posRot)//, Ragdoll *ragdoll)
	: mWorld(world), mRagdoll(nullptr), mLife(100.f), mIsAlive(true)
	, mDynamicBox(nullptr)
{
	// Vérifie les objets passés
	assert(mWorld && "World invalid!");

	// Change la texture du sprite
	mSprite = new sf::Sprite;
	mSprite->setTexture(*texture);

	// Crée la DynamicBox
	mDynamicBox = new DynamicBox(mWorld, posRot, mSprite);
	mDynamicBox->SetType(BodyType::Entity);
	mDynamicBox->SetEntity(this);
	mDynamicBox->GetBody()->SetFixedRotation(true);
	mWorld->RegisterBody(mDynamicBox);

	// Initialise l'animation
	mAnimation.addAnimation(1, testAnimation, sf::seconds(5));
	mAnimation.playAnimation(1, true);
	mAnimation.animate(*mSprite);
	
	// Règle la DynamicBox pour correspondre à l'animation
	mDynamicBox->SetSize(40.f * mWorld->GetMPP(), 150.f * mWorld->GetMPP());
	mElapsedTime.restart();
}
Entity::~Entity(void)
{
	if (IsAlive())
		mWorld->DestroyBody(mDynamicBox);
	else
		delete mRagdoll;
	delete mSprite;
}

// Met à jour le body
void Entity::Update()
{
	if (mLife == 0.f && mIsAlive == true)
	{
		// Supprime la DynamicBox / Sprite
		b2Vec2 pos = mDynamicBox->GetBodyPosition();
		float rot = mDynamicBox->GetBodyAngle();
		mWorld->DestroyBody(mDynamicBox);

		// Crée le ragdoll
		mRagdoll = new Ragdoll(mWorld, b2Vec3(pos.x, pos.y, rot));

		mIsAlive = false;
	}
	else if (mIsAlive)
	{
		mAnimation.update(mElapsedTime.getElapsedTime());
		mElapsedTime.restart();
		mAnimation.animate(*mSprite);
	}
}

// Change les composants
void Entity::SetSprite(sf::Sprite *newSprite)
{
	mSprite = newSprite;
}

// Change les attributs de l'Entité
void Entity::Kill()
{
	mLife = 0.f;
}
void Entity::Heal(float amount)
{
	mLife += amount;
	mLife = ((mLife > 100.f) ? 100.f : ((mLife < 0.f) ? 0.f : mLife));
}
void Entity::AddDamage(float damage)
{
	mLife -= damage;
	mLife = ((mLife > 100.f) ? 100.f : ((mLife < 0.f) ? 0.f : mLife));
	std::cout << mLife << std::endl;
}

// Accesseur
bool Entity::IsAlive() const
{
	return mIsAlive;
}
float Entity::GetLife() const
{
	return mLife;
}
