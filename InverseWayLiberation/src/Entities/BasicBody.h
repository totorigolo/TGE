#pragma once
#include "BaseBody.h"

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
	BasicBody(int layer = 1, unsigned int ID = 0U);

	// Cr�ation du body
	bool CreateDynBox(b2Vec3 posRot, const std::shared_ptr<Texture> texture,
					  float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
					  int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateDynCircle(b2Vec3 posRot, const std::shared_ptr<Texture> texture,
						 float density = 1.f, float friction = 0.2f, float restitution = 0.f,
						 int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateStaticBox(b2Vec3 posRot, const std::shared_ptr<Texture> texture,
						 float density = 0.f, float friction = 0.2f, float restitution = 0.0f,
						 int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	bool CreateStaticCircle(b2Vec3 posRot, const std::shared_ptr<Texture> texture,
							float density = 0.f, float friction = 0.2f, float restitution = 0.0f,
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