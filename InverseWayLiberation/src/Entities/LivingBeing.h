#pragma once
#include "Entity.h"
#include "../Physics/World.h"
#include "../Physics/Body.h"
#include "../Resources/AnimatedSprite.h"
#include <string>

class LivingBeing : public Entity
{
public:
	// Ctor & dtor
	LivingBeing(World *world, b2Vec2 position, std::shared_ptr<sf::Texture> texture, int layer = 1);
	virtual ~LivingBeing();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// ContactListener uniquement : indique si l'entité peut sauter
	void CanJump(bool b);

	// Accesseurs
	Body* GetCollisionBody();
	const Body* GetCollisionBody() const;
	//AnimatedSprite& GetAnimatedSprite();
	//const AnimatedSprite& GetAnimatedSprite() const;
	
protected:
	bool mIsDead;
	bool mCanJump;

	World *mWorld;
	Body *mCollisionBody;

	sf::Sprite mSprite;
	std::shared_ptr<sf::Texture> mTexture;

	//AnimatedSprite mAnimatedSprite;
};