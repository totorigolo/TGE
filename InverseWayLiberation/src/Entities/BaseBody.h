#pragma once
#include "Entity.h"
#include "../Resources/Texture.h"

class PhysicManager;
class BaseBody : public Entity
{
public:
	// Propri�t� de collision
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

	// Mise � jour
	virtual void Update();

	// Destruction du body
	virtual void Destroy();

	/* Accesseurs */
	// Type de b2Body
	b2BodyType Getb2BodyType() const;
	void Setb2BodyType(const b2BodyType &type);
	// Type de collision
	void SetCollisionType(CollisionType type);
	CollisionType GetCollisionType() const;
	// Param�tres de collision
	float GetDensity() const;
	void SetDensity(float density);
	float GetFriction() const;
	void SetFriction(float friction);
	float GetRestitution() const;
	void SetRestitution(float restitution);
	// Texture
	std::shared_ptr<Texture> GetTexture();
	void SetTexture(std::shared_ptr<Texture> texture);
	// Sprite
	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;
	// Body
	b2Body* GetBody();
	const b2Body* GetBody() const;
	// Position & rotation
	const b2Vec2 GetPosition() const;
	const float GetRotationD() const; // Degr�s
	const float GetRotationR() const; // Radians
	// Fonction � n'employer que pour �diter les niveaux
	void SetTransform(const b2Vec2 &position, float angle = 0.f);

protected:
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	// Propri�t� de collision
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