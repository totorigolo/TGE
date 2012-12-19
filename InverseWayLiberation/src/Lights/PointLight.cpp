#include "PointLight.h"
#include "../Tools/utils.h"

// Ctor & Dtor
PointLight::PointLight(sf::Vector2f pos, float radius, bool isStatic, bool isActivated, Body* emitter)
	: Light(pos, radius, isStatic, isActivated, emitter)
{
	SetRadius(radius);

	// Règle la lumière
	mLight.setFillColor(sf::Color(255, 255, 0, 128));
}
PointLight::PointLight(float x, float y, float radius, bool isStatic, bool isActivated, Body* emitter)
	: Light(x, y, radius, isStatic, isActivated, emitter)
{
	SetRadius(radius);

	// Règle la lumière
	mLight.setFillColor(sf::Color(255, 255, 0, 128));
}
PointLight::~PointLight()
{
}

// Règle la lumière
void PointLight::SetRadius(float radius)
{
	// Sauvegarde le rayon
	mRadius = radius;

	// Change le rayon de la lumière
	mLight.setRadius(radius);

	// Recrée une nouvelle texture de rendu pour cette lampe
	mRenderTexture.create(static_cast<unsigned int>(radius) * 2U, static_cast<unsigned int>(radius) * 2U);

	// Met à jour le sprite de la texture de rendu
	mSprite.setTexture(mRenderTexture.getTexture());
	mSprite.setOrigin(mSprite.getGlobalBounds().width / 2.f, mSprite.getGlobalBounds().height / 2.f);

	// Met à jour la taille de la vue
	mView.setSize(u2f(mRenderTexture.getSize()));
}

// Affichage de la lumière
void PointLight::RenderLight()
{
	// On oublie que la lampe est cachée quand on la redessine
	mIsHidden = false;

	// Dessine le "rond" à la vue initiale
	mRenderTexture.clear(sf::Color(0, 0, 0, 0));
	mRenderTexture.setView(mRenderTexture.getDefaultView());
	mRenderTexture.draw(mLight);
	mRenderTexture.setView(mView);
}

// Récupérer l'AABB de la lumière
sf::FloatRect PointLight::GetAABB() const
{
	return mSprite.getGlobalBounds();
}
