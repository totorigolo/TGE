#include "PointLight.h"

// Ctor & Dtor
PointLight::PointLight(sf::Vector2f pos, float radius, bool isStatic, bool isActivated, Body* emitter)
	: Light(pos, radius, isStatic, isActivated, emitter)
{
	// R�gle la lumi�re
	mLight.setFillColor(sf::Color(255, 255, 0, 128));
	SetRadius(mRadius);
}
PointLight::PointLight(float x, float y, float radius, bool isStatic, bool isActivated, Body* emitter)
	: Light(x, y, radius, isStatic, isActivated, emitter)
{
	// R�gle la lumi�re
	mLight.setFillColor(sf::Color(255, 255, 0, 128));
	SetRadius(mRadius);
}
PointLight::~PointLight()
{
}

// R�gle la lumi�re
void PointLight::SetRadius(float radius)
{
	Light::SetRadius(radius);

	mLight.setRadius(mRadius);
	mLight.setOrigin(mLight.getGlobalBounds().width / 2.f, mLight.getGlobalBounds().height / 2.f);
}

// Mise � jour
void PointLight::Update()
{
	Light::Update();

	mLight.setPosition(mPosition);
}

// R�cup�rer l'AABB de la lumi�re
sf::FloatRect PointLight::GetAABB() const
{
	return mLight.getGlobalBounds();
}

// Affichage
void PointLight::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsActivated)
		target.draw(mLight, states);
}
