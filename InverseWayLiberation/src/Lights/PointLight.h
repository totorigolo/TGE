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

	// Mise à jour
	virtual void Update();

	// Récupérer l'AABB de la lumière
	virtual sf::FloatRect GetAABB() const;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	// Affichage
	sf::CircleShape mLight;
};
