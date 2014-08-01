#pragma once
#include "BaseBody.h"

class PolyChain : public BaseBody
{
public:
	enum Type {
		None = 0,
		Chain,
		Loop
	};

public:
	// Ctor & dtor
	PolyChain(int layer = 1);

	// Mise à jour
	void PreUpdate();

	// Création du body, centre calculé par rapport aux points
	bool Create(const std::vector<b2Vec2> &vertices, Type type, Texture::Ptr texture,
				float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
				int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	// Création du body, coordonnées des pts par rapport au centre, sans rotation (appliquée après)
	bool Create(b2Vec3 posRot, const std::vector<b2Vec2> &vertices, Type type, Texture::Ptr texture,
				float density = 1.f, float friction = 0.2f, float restitution = 0.0f,
				int groupIndex = 0, uint16 categoryBits = 0x0001, uint16 maskBits = 0xFFFF);

	// Vérifie les points passés
	bool CheckPoints(const std::vector<b2Vec2> &vertices);

	/* Accesseurs */
	// Points du Body
	const std::vector<b2Vec2>& GetPoints() const;
	// Type
	Type GetChainType() const;

private:
	// Type
	Type mChainType;

	// Points du Poly
	std::vector<b2Vec2> mPoints;
};
