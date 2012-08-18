#include "Body.h"
#include "config.h"
#include "utils.h"

//Ctor
Body::Body(b2World *world, std::shared_ptr<sf::Texture> texture)
	: mWorld(world), mBody(nullptr), mTexture(texture)
{
	// Change la texture
	this->setTexture(*mTexture);
}

// Dtor
Body::~Body(void)
{
}

// Met à jour la position du sprite
void Body::Update()
{
	if (mBody && mWorld)
	{
		// "-y" parce que la SFML inverse
		this->setPosition(mBody->GetPosition().x * PPM, - mBody->GetPosition().y * PPM);
		this->setOrigin(u2f(mTexture->getSize()) / 2.f);
		this->setRotation(mBody->GetAngle() * DPR);
	}
}

// Retourne true si l'objet est trop loin
bool Body::IsInRange(b2Vec2 const& xw, b2Vec2 const& yh) const
{
	if (mBody)
		if (mBody->GetPosition().x < xw.x && mBody->GetPosition().x > xw.y && mBody->GetPosition().y < yh.x && mBody->GetPosition().y > yh.y)
			return true;
	return false;
}

void Body::Destroy()
{
	if (mWorld && mBody)
		mWorld->DestroyBody(mBody);
}