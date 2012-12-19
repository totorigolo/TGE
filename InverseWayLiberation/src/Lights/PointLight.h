#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	// Ctor & Dtor
	PointLight(sf::Vector2f pos, float radius, bool isStatic, bool isActivated = true, Body* emitter = nullptr);
	PointLight(float x, float y, float radius, bool isStatic, bool isActivated = true, Body* emitter = nullptr);
	virtual ~PointLight();
	
	// Règle la lumière
	virtual void SetRadius(float radius);

	// Affichage de la lumière
	virtual void RenderLight();

	// Récupérer l'AABB de la lumière
	virtual sf::FloatRect GetAABB() const;

private:
	// Affichage
	sf::CircleShape mLight;
};
