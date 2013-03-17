#pragma once
#include <Box2D/Box2D.h>
#include <string>
#include "Entity.h"
#include "../Physics/PhysicManager.h"
#include "../Resources/AnimatedSprite.h"

class LivingBeing : public Entity
{
public:
	// Ctor & dtor
	LivingBeing(PhysicManager *physicMgr, b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer = 1);
	virtual ~LivingBeing();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// ContactListener uniquement : indique si l'entité peut sauter
	void CanJump(bool b);

	// Gestion des dépendences
	virtual void DependencyDestroyed(void *dependency);

	// Accesseurs
	b2Body* GetCollisionBody();
	const b2Body* GetCollisionBody() const;
	//AnimatedSprite& GetAnimatedSprite();
	//const AnimatedSprite& GetAnimatedSprite() const;
	
protected:
	bool mIsDead;
	bool mCanJump;

	PhysicManager *mPhysicMgr;
	b2Body *mCollisionBody;

	sf::Sprite mSprite;
	std::shared_ptr<sf::Texture> mTexture;

	//AnimatedSprite mAnimatedSprite;
};