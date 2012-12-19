#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	// Ctor & Dtor
	PointLight(sf::Vector2f pos, float radius, bool isStatic, bool isActivated = true, Body* emitter = nullptr);
	PointLight(float x, float y, float radius, bool isStatic, bool isActivated = true, Body* emitter = nullptr);
	virtual ~PointLight();
	
	// R�gle la lumi�re
	virtual void SetRadius(float radius);

	// Affichage de la lumi�re
	virtual void RenderLight();

	// R�cup�rer l'AABB de la lumi�re
	virtual sf::FloatRect GetAABB() const;

private:
	// Affichage
	sf::CircleShape mLight;
};
