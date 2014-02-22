#pragma once
#include "Entity.h"
#include "../Resources/Texture.h"

class PhysicManager;
class LivingBeing : public Entity
{
public:
	// Ctor & dtor
	LivingBeing(b2Vec2 position, const std::shared_ptr<Texture> &texture, int layer = 1, unsigned int ID = 0U);
	virtual ~LivingBeing();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Accesseurs
	b2Body* GetCollisionBody();
	const b2Body* GetCollisionBody() const;
	const sf::Sprite* GetSprite() const;
	const b2Vec2 GetPosition() const;
	// Body
	b2Body* GetBody();
	const b2Body* GetBody() const;
	
protected:
	bool mIsDead;
	bool mCanJump;

	PhysicManager &mPhysicMgr;
	b2Body *mBody;
	bool mBodyIsCreated;

	sf::Sprite mSprite;
	std::shared_ptr<Texture> mTexture;
};