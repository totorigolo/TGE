#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include <list>
#include "../World.h"
#include "../Joints/Joint.h"

enum BodyType
{
	body_fullySimulated = 0,
	body_actor,
	body_oneSidedPlatform
};

class World;
class Joint;
class Body : public sf::Sprite
{
public:
	// Ctor & dtor
	Body(World *world, BodyType type = body_fullySimulated);
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

	// Accesseurs
	void SetBody(b2Body *body); // NE PAS UTILISER
	inline void SetDrawable(bool drawable) { mIsDrawable = drawable; }
	
	void SetType(BodyType type);
	inline BodyType GetType() const { return mType; }
	
	virtual b2AABB GetBodyAABB() const; // = 0; // Retourne une AABB nulle
	b2Vec2 GetBodySize() const;
	float GetBodyAngle() const;
	b2Vec2 GetBodyPosition() const;

	inline bool IsNull() const { return mIsNull; }
	inline bool IsDrawable() const { return mIsDrawable; }
	inline bool IsStatic() const { return mBody->GetType() == b2_staticBody; }
	inline b2Body* GetBody() { return mBody; }
	inline World* GetWorld() { return mWorld; }
	inline b2Shape* GetShape() { return mShape; }
	inline b2Body const* GetBody() const { return mBody; }
	inline World const* GetWorld() const { return mWorld; }
	inline b2Shape const* GetShape() const { return mShape; }

protected:
	BodyType mType;

	bool mIsNull;
	bool mIsDrawable;

	// Variables pour l'objet physique
	World *mWorld;
	b2Body *mBody;
	b2Shape *mShape;

	// Liste des joint auxquels il est attaché
	std::list<Joint*> mJointList;
};
