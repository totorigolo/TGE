#include "PointLight.h"
#include "../Tools/utils.h"

// Ctor & Dtor
PointLight::PointLight(sf::Vector2f pos, float radius, bool isStatic, bool isActivated, Body* emitter)
	: Light(pos, radius, isStatic, isActivated, emitter)
{
	SetRadius(radius);

	// R�gle la lumi�re
	mLight.setFillColor(sf::Color(255, 255, 0, 128));
}
PointLight::PointLight(float x, float y, float radius, bool isStatic, bool isActivated, Body* emitter)
	: Light(x, y, radius, isStatic, isActivated, emitter)
{
	SetRadius(radius);

	// R�gle la lumi�re
	mLight.setFillColor(sf::Color(255, 255, 0, 128));
}
PointLight::~PointLight()
{
}

// R�gle la lumi�re
void PointLight::SetRadius(float radius)
{
	// Sauvegarde le rayon
	mRadius = radius;

	// Change le rayon de la lumi�re
	mLight.setRadius(radius);

	// Recr�e une nouvelle texture de rendu pour cette lampe
	mRenderTexture.create(static_cast<unsigned int>(radius) * 2U, static_cast<unsigned int>(radius) * 2U);

	// Met � jour le sprite de la texture de rendu
	mSprite.setTexture(mRenderTexture.getTexture());
	mSprite.setOrigin(mSprite.getGlobalBounds().width / 2.f, mSprite.getGlobalBounds().height / 2.f);

	// Met � jour la taille de la vue
	mView.setSize(u2f(mRenderTexture.getSize()));
}

// Affichage de la lumi�re
void PointLight::RenderLight()
{
	// On oublie que la lampe est cach�e quand on la redessine
	mIsHidden = false;

	// Dessine le "rond" � la vue initiale
	mRenderTexture.clear(sf::Color(0, 0, 0, 0));
	mRenderTexture.setView(mRenderTexture.getDefaultView());
	mRenderTexture.draw(mLight);
	mRenderTexture.setView(mView);
}

// R�cup�rer l'AABB de la lumi�re
sf::FloatRect PointLight::GetAABB() const
{
	return mSprite.getGlobalBounds();
}
