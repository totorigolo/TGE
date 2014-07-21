#pragma once
#include "BaseBody.h"

class PolyBody : public BaseBody
{
public:
	// Ctor & dtor
	PolyBody(int layer = 1, unsigned int ID = 0U);

	// Mise � jour
	void Update();

	// Cr�ation du body, centre calcul� par rapport aux points
	bool Create(const std::vector<b2Vec2> &vertices, b2BodyType type, Texture::Ptr texture,
				float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
				int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	// Cr�ation du body, coordonn�es des pts par rapport au centre, sans rotation (appliqu�e apr�s)
	bool Create(b2Vec3 posRot, const std::vector<b2Vec2> &vertices, b2BodyType type, Texture::Ptr texture,
				float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
				int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	// V�rifie les points pass�s
	bool CheckPoints(const std::vector<b2Vec2> &vertices);

	/* Accesseurs */
	// Points du Body
	const std::vector<b2Vec2>& GetPoints() const;

private:
	// Points du Poly
	std::vector<b2Vec2> mPoints;
};
