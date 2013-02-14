#include "RawBody.h"
#include "../Physics/World.h"

// Ctor & dtor
RawBody::RawBody(Body *body, int layer)
	: Entity(layer), mBody(body)
{
	mType = EntityType::RawBody;

	if (mBody)
	{
		mBody->SetEntity(this);
	}
}
RawBody::~RawBody()
{
	if (mBody)
	{
		// Supprime le Body
		mBody->GetWorld()->DestroyBody(mBody);
		mBody = nullptr;
	}
}

// Mise à jour
void RawBody::Update()
{
	if (mBody)
	{
		mBody->Update();
	}
	// TODO: Else, suppression automatique
}

// Pour le rendu
void RawBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mBody)
	{
		target.draw(*mBody, states);
	}
}

// Accesseurs
Body* RawBody::GetBody()
{
	return mBody;
}
const Body* RawBody::GetBody() const
{
	return mBody;
}
