#include "Light.h"
#include "../Tools/utils.h"
#include <iostream>

// Ctor & Dtor
Light::Light(sf::Vector2f pos, float radius, bool isStatic, bool isActivated, Body* emitter)
	: mPosition(pos), mRadius(radius), mIsStatic(isStatic), mIsActivated(isActivated), mIsHidden(false),
	mEmitter(emitter)
{
	SetRadius(mRadius);

	if (mEmitter)
		mPosRelEmitter = pos;
	UpdatePosition();
}
Light::Light(float x, float y, float radius, bool isStatic, bool isActivated, Body* emitter)
	: mPosition(x, y), mRadius(radius), mIsStatic(isStatic), mIsActivated(isActivated), mIsHidden(false),
	mEmitter(emitter)
{
	SetRadius(mRadius);

	if (mEmitter)
		mPosRelEmitter = sf::Vector2f(x, y);
	UpdatePosition();
}
Light::~Light()
{
}

// Gestion des propri�t�s
bool Light::IsStatic() const
{
	return mIsStatic;
}
void Light::SetStatic(bool isStatic)
{
	mIsStatic = isStatic;
}
bool Light::IsActivated() const
{
	return mIsActivated;
}
void Light::Activate()
{
	mIsActivated = true;
}
void Light::Deactivate()
{
	mIsActivated = false;
}
void Light::SetActivated(bool activated)
{
	mIsActivated = activated;
}

// D�placement de la lumi�re
void Light::SetPosition(sf::Vector2f pos)
{
#ifdef _DEBUG
	if (mIsStatic)
		std::cout << "StaticLight moved !!" << std::endl;
#endif

	mPosition = pos;
	UpdatePosition();
}
void Light::Move(float dx, float dy)
{
#ifdef _DEBUG
	if (mIsStatic)
		std::cout << "StaticLight moved !!" << std::endl;
#endif

	mPosition.x += dx;
	mPosition.y += dy;
	UpdatePosition();
}
void Light::Move(const sf::Vector2f& dep)
{
#ifdef _DEBUG
	if (mIsStatic)
		std::cout << "StaticLight moved !!" << std::endl;
#endif

	mPosition += dep;
	UpdatePosition();
}

// Mise � jour de la position
void Light::UpdatePosition()
{
	// La position est relative au body, mais ne bouge pas si la lumi�re est statique
	if (mEmitter && !mIsStatic)
	{
		mPosition = b22sfVec(mEmitter->GetBodyPosition(), mEmitter->GetWorld()->GetPPM()) + mPosRelEmitter;
	}
	mSprite.setPosition(mPosition);

	mView.setCenter(mPosition);
	mRenderTexture.setView(mView);
}

// Affichage des ombres de la texture de la lumi�re
void Light::DisplayTexture()
{
	mRenderTexture.display();
}
void Light::DrawOn(const sf::Drawable &drawable, sf::RenderStates states)
{
	mRenderTexture.draw(drawable, states);
}

// La lumi�re est *dans* un objet
bool Light::IsHidden() const
{
	return mIsHidden;
}
void Light::IsHidden(bool hiden)
{
	mIsHidden = hiden;
}

// R�gle la lumi�re
void Light::SetRadius(float radius)
{
	mRadius = radius;
}
float Light::GetRadius() const
{
	return mRadius;
}

void Light::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mIsActivated && !mIsHidden)
		target.draw(mSprite, states);
}
	