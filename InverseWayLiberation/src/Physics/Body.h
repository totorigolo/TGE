#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>
#include <list>
#include "World.h"
#include "Joint.h"

class World;
class Joint;
class Body : public sf::Sprite
{
public:
	// Ctor & dtor
	Body(World *world);
	virtual ~Body(void);
	
	// Met à jour le body
	virtual void Update();

	// Retourne true si l'objet est trop loin
	bool IsInRange(b2Vec2 const& xw, b2Vec2 const& yh) const;

	// Gestion des joints
	void RegisterJoint(Joint *joint);
	void RemoveJoint(Joint *joint);
	void DestroyJoint(Joint *joint, bool remove = true);
	void DestroyAllJoints();

	// Accesseurs
	void SetBody(b2Body *body); // NE PAS UTILISER
	void SetDrawable(bool drawable) { mIsDrawable = drawable; }

	bool IsNull() const { return mIsNull; }
	bool IsDrawable() const { return mIsDrawable; }
	bool IsStatic() const { return mBody->GetType() == b2_staticBody; }
	b2Body* GetBody() { return mBody; }
	World* GetWorld() { return mWorld; }
	b2Shape* GetShape() { return mShape; }
	b2Body const* GetBody() const { return mBody; }
	World const* GetWorld() const { return mWorld; }
	b2Shape const* GetShape() const { return mShape; }

protected:
	bool mIsNull;
	bool mIsDrawable;

	// Variables pour l'objet physique
	World *mWorld;
	b2Body *mBody;
	b2Shape *mShape;

	// Liste des joint auxquels il est attaché
	std::list<Joint*> mJointList;
};
