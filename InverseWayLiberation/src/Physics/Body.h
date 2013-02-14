#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include <list>
#include "World.h"
#include "Joint.h"

enum class BodyType
{
	FullySimulated = 0,
	Bullet,
	OneSidedPlatform
};

class Hull;
class World;
class Joint;
class Entity;
class Body : public sf::Drawable
{
public:
	// Ctor & dtor
	Body(World *world, BodyType type = BodyType::FullySimulated);
	virtual ~Body(void);
	
	// Met à jour le body
	virtual void Update();

	// Retourne true si l'objet est dedans
	bool IsInRange(b2Vec2 const& xw, b2Vec2 const& yh) const;

	// Gestion des joints
	void RegisterJoint(Joint *joint);
	void RemoveJoint(Joint *joint);
	void DestroyJoint(Joint *joint, bool remove = true);
	void DestroyAllJoints();

	// Accesseurs //
	// Gestion du sprite
	void SetSprite(sf::Sprite *sprite);
	const sf::Sprite* GetSprite() const;

	// Gestion du type
	void SetType(BodyType type);
	inline BodyType GetType() const { return mType; }

	// Gestion de l'Entity associée
	void SetEntity(Entity *entity);
	inline Entity* GetEntity() { return mEntity; }
	inline const Entity* GetEntity() const { return mEntity; }

	// Gestion des propriétés Box2D
	virtual b2AABB GetBodyAABB() const = 0; // Retourne une AABB nulle
	b2Vec2 GetBodySize() const;
	float GetBodyAngle() const;
	b2Vec2 GetBodyPosition() const;

	// Gestion des propriétés
	inline void SetDrawable(bool drawable) { mIsDrawable = drawable; }
	
	// Accès aux propriétés
	inline bool IsNull() const { return mIsNull; }
	inline bool IsDrawable() const { return mIsDrawable; }
	inline bool IsStatic() const { return mBody->GetType() == b2_staticBody; }
	inline b2Body* GetBody() { return mBody; }
	inline World* GetWorld() { return mWorld; }
	inline b2Body const* GetBody() const { return mBody; }
	inline World const* GetWorld() const { return mWorld; }
	inline bool IsSleeping() const
	{
		return !mBody->IsAwake();
	}
	
	// Gestion des ombres
	void SetHull(Hull *hull); // Faire attention à supprimer / ajouter le Hull au LightManager !
	Hull* GetHull() { return mHull; }
	const Hull* GetHull() const { return mHull; }
	
	// NE PAS UTILISER
	void SetBody(b2Body *body);

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

protected:
	BodyType mType;

	bool mIsNull;
	bool mIsDrawable;

	// Variables pour l'objet physique
	World *mWorld;
	b2Body *mBody;

	// Gestion du sprite
	sf::Sprite *mSprite;
	bool mItsMySprite;

	// Liste des joint auxquels il est attaché
	std::list<Joint*> mJointList;

	// Entity
	Entity *mEntity;

	// Lumières
	Hull* mHull;
};
