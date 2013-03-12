#pragma once
#include <Box2D/Box2D.h>
#include "Entity.h"
#include "../Physics/PhysicManager.h"

enum class BasicBodyType
{
	Null,
	DynamicBox,
	DynamicCircle,
	StaticBox
};

class BasicBody : public Entity
{
public:
	// Ctor & dtor
	BasicBody(PhysicManager *mgr, int layer = 1);
	virtual ~BasicBody();

	// Cr�ation du body
	bool CreateDynBox(b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float density = 1.f, float friction = 0.2f, float restitution = 0.0f
																		 , int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateDynCircle(b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float density = 1.f, float friction = 0.2f, float restitution = 0.f
																			, int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateStaticBox(b2Vec3 posRot, std::shared_ptr<sf::Texture> texture, float friction = 0.2f, float restitution = 0.0f
																			, int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	// Mise � jour
	virtual void Update();

	// Destruction du body
	void Destroy();

	/* Accesseurs */
	// Type de BasicBody
	BasicBodyType GetBasicBodyType() const;
	// Sprite
	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;
	// Body
	b2Body* GetBody();
	const b2Body* GetBody() const;
	// PhysicManager
	PhysicManager* GetPhysicManager();
	const PhysicManager* GetPhysicManager() const;

protected:
	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Type de body
	BasicBodyType mBasicBodyType;

	// PhysicManager
	PhysicManager *mPhysicMgr;

	// Le body Box2D
	bool mBodyIsCreated;
	b2Body *mBody;

	// Sprite SFML
	sf::Sprite mSprite;
};