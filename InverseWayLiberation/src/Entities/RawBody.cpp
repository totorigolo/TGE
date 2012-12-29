#include "RawBody.h"
#include "../Physics/World.h"

// Ctor & dtor
RawBody::RawBody(Body *body, int layer)
	: Entity(layer), mBody(body)
{
	mType = EntityType::RawBody;

	mBody->SetEntity(this);
}
RawBody::~RawBody()
{
	// Supprime le Body
	mBody->GetWorld()->DestroyBody(mBody);
	mBody = nullptr;
}

// Mise à jour
void RawBody::Update()
{
	mBody->Update();
}

// Pour le rendu
void RawBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*mBody, states);
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
