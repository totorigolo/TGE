#include "Light.h"
#include <iostream>

// Ctor & Dtor
Light::Light(sf::Vector2f pos, float radius, bool isStatic, bool isActivated, Body* emitter)
	: mPosition(pos), mRadius(radius), mIsStatic(isStatic), mIsActivated(isActivated), mEmitter(emitter)
{
	SetRadius(mRadius);
}
Light::Light(float x, float y, float radius, bool isStatic, bool isActivated, Body* emitter)
	: mPosition(x, y), mRadius(radius), mIsStatic(isStatic), mIsActivated(isActivated), mEmitter(emitter)
{
	SetRadius(mRadius);
}
Light::~Light()
{
}

// Gestion des propriétés
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

// Déplacement de la lumière
void Light::SetPosition(sf::Vector2f pos)
{
#ifdef _DEBUG
	if (mIsStatic)
		std::cout << "StaticLight moved !!" << std::endl;
#endif

	mPosition = pos;
	Update();
}
void Light::Move(float dx, float dy)
{
#ifdef _DEBUG
	if (mIsStatic)
		std::cout << "StaticLight moved !!" << std::endl;
#endif

	mPosition.x += dx;
	mPosition.y += dy;
	Update();
}
void Light::Move(const sf::Vector2f& dep)
{
#ifdef _DEBUG
	if (mIsStatic)
		std::cout << "StaticLight moved !!" << std::endl;
#endif

	mPosition += dep;
	Update();
}

// Règle la lumière
void Light::SetRadius(float radius)
{
	mRadius = radius;
}
float Light::GetRadius() const
{
	return mRadius;
}
