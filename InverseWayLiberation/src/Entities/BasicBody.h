#pragma once
#include "Entity.h"
#include "../Resources/Texture.h"

#include <Box2D/Box2D.h>
#include <memory>

class PhysicManager;
class BasicBody : public Entity
{
public:
	// Forme du Body
	enum class Shape
	{
		Null,
		Box,
		Circle
	};

	// Propriété de collision
	enum class CollisionType
	{
		Default,
		Bullet,
		OneSidedPlatform
	};

public:
	// Ctor & dtor
	BasicBody(int layer = 1, unsigned int ID = 0U);
	virtual ~BasicBody();

	// Création du body
	bool CreateDynBox(b2Vec3 posRot, const std::shared_ptr<Texture> &texture,
					  float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
					  int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateDynCircle(b2Vec3 posRot, const std::shared_ptr<Texture> &texture,
						 float density = 1.f, float friction = 0.2f, float restitution = 0.f,
						 int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateStaticBox(b2Vec3 posRot, const std::shared_ptr<Texture> &texture,
						 float density = 0.f, float friction = 0.2f, float restitution = 0.0f,
						 int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateStaticCircle(b2Vec3 posRot, const std::shared_ptr<Texture> &texture,
							float density = 0.f, float friction = 0.2f, float restitution = 0.0f,
							int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	// Mise à jour
	virtual void Update();

	// Destruction du body
	void Destroy();

	/* Accesseurs */
	// Forme du Body
	Shape GetBasicBodyShape() const;
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
	// Forme du BasicBody
	Shape mShape;

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