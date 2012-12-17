#pragma once
#include "Light.h"

class SpotLight : public Light
{
public:
	// Ctor & Dtor
	SpotLight();
	virtual ~SpotLight();
	
	// R�cup�rer l'AABB de la lumi�re
	virtual sf::FloatRect GetAABB() const;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	float mRadius;
};
