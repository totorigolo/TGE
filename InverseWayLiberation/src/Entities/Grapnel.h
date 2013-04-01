#pragma once
#include <SFML/System.hpp>
#include <Box2D/Box2D.h>
#include <memory>
#include "Entity.h"

class Joint;
class PhysicManager;
class DistanceJoint;
class Grapnel : public Entity
{
public:
	// Ctor & dtor
	Grapnel(int layer = 1);
	virtual ~Grapnel();

	// Création du grapin
	bool Create(const std::shared_ptr<sf::Texture> &textureHook, b2Body *bodyA, b2Vec2 ptA, b2Body *bodyB, b2Vec2 ptB);

	// Destruction du grappin
	void Destroy();

	// Mise à jour
	virtual void Update();

	// Accesseurs
	Joint* GetJoint();
	const Joint* GetJoint() const;
	PhysicManager* GetPhysicManager();
	const PhysicManager* GetPhysicManager() const;

protected:
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// PhysicManager
	PhysicManager &mPhysicMgr;

	// Le joint Box2D
	int mJointID;
	DistanceJoint *mJoint;

	// Rétractation
	bool mIsRetracting;
	sf::Clock mClock;

	// Ressources SFML
	sf::Sprite mSpriteHook;
	std::shared_ptr<sf::Texture> mTexture;
};