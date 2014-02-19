#pragma once
#include "BaseBody.h"

class PolyBody : public BaseBody
{
public:
	// Ctor & dtor
	PolyBody(int layer = 1, unsigned int ID = 0U);

	// Création du body
	bool Create(std::vector<b2Vec2> vertices, b2BodyType type, const std::shared_ptr<Texture> &texture,
				float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
				int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);
};