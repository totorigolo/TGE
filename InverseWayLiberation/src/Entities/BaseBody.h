#pragma once
#include "Entity.h"
#include "../Resources/Texture.h"

#include <Box2D/Box2D.h>
#include <memory>

class PhysicManager;
class BaseBody : public Entity
{
public:
	// Propriété de collision
	enum class CollisionType
	{
		Default,
		Bullet,
		OneSidedPlatform
	};

public:
	// Ctor & dtor
	BaseBody(int layer = 1, unsigned int ID = 0U);
	virtual ~BaseBody();

	// Mise à jour
	virtual void Update();

	// Destruction du body
	void Destroy();

	/* Accesseurs */
	// Type de b2Body
	b2BodyType Getb2BodyType() const;
	void Setb2BodyType(const b2BodyType &type);
	// Type de collision
	void SetCollisionType(CollisionType type);
	CollisionType GetCollisionType() const;
	// Paramètres de collision
	float GetDensity() const;
	void SetDensity(float density);
	float GetFriction() const;
	void SetFriction(float friction);
	float GetRestitution() const;
	void SetRestitution(float restitution);
	// Sprite
	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;
	// Body
	b2Body* GetBody();
	const b2Body* GetBody() const;
	// Position & rotation
	const b2Vec2 GetPosition() const;
	const float GetRotationD() const; // Degrés
	const float GetRotationR() const; // Radians
	// Fonction à n'employer que pour éditer les niveaux
	void SetTransform(const b2Vec2 &position, float angle = 0.f);

protected:
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Propriété de collision
	CollisionType mCollisionType;

	// PhysicManager
	PhysicManager &mPhysicMgr;

	// Le body Box2D
	bool mBodyIsCreated;
	b2Body *mBody;

	// Sprite SFML
	sf::Sprite mSprite;
	std::shared_ptr<Texture> mTexture;
};