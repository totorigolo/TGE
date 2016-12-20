#pragma once
#include "BaseBody.h"
#include "Hull.h"

class PhysicManager;
class BasicBody : public BaseBody
{
public:
	// Forme du Body
	enum class Shape
	{
		Null,
		Box,
		Circle
	};

public:
	// Ctor & dtor
	BasicBody(int layer = 1);

	// Mise à jour
	virtual void PreUpdate();

	// Création du body
	bool CreateBox(b2Vec3 posRot, b2BodyType type, Texture::Ptr texture,
					  float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
					  int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateCircle(b2Vec3 posRot, b2BodyType type, Texture::Ptr texture,
						 float density = 1.f, float friction = 0.2f, float restitution = 0.f,
						 int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	// Destruction du body
	virtual void Destroy();

	/* Accesseurs */
	// Forme du Body
	Shape GetBasicBodyShape() const;

private:
	// Forme du BasicBody
	Shape mShape;
};