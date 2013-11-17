#pragma once
#include "Entity.h"
#include "../Resources/Texture.h"

#include <Box2D/Box2D.h>
#include <memory>

class PhysicManager;
class BasicBody : public Entity
{
public:
	// Type de Body
	enum class Type
	{
		Null,
		DynamicBox,
		DynamicCircle,
		StaticBox
	};

	// Propri�t� de collision
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

	// Cr�ation du body
	bool CreateDynBox(b2Vec3 posRot, const std::shared_ptr<Texture> &texture,
					  float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
					  int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateDynCircle(b2Vec3 posRot, const std::shared_ptr<Texture> &texture,
						 float density = 1.f, float friction = 0.2f, float restitution = 0.f,
						 int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateStaticBox(b2Vec3 posRot, const std::shared_ptr<Texture> &texture,
						 float friction = 0.2f, float restitution = 0.0f,
						 int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	// Mise � jour
	virtual void Update();

	// Destruction du body
	void Destroy();

	/* Accesseurs */
	// Type de BasicBody
	Type GetBasicBodyType() const;
	// Type de collision
	void SetCollisionType(CollisionType type);
	CollisionType GetCollisionType() const;
	// Sprite
	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;
	// Body
	b2Body* GetBody();
	const b2Body* GetBody() const;
	// Position & rotation (en degr�s)
	const b2Vec2 GetPosition() const;
	const float GetRotation() const;
	// Fonction � n'employer que pour �diter les niveaux
	void SetTransform(const b2Vec2 &position, float angle = 0.f);

protected:
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Type de body
	Type mBasicBodyType;
	
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