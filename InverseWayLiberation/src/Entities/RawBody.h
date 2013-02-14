#pragma once
#include "Entity.h"
#include "../Physics/Body.h"

class RawBody : public Entity
{
public:
	// Ctor & dtor
	RawBody(Body *body, int layer = 1);
	virtual ~RawBody();

	// Mise à jour
	virtual void Update();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Accesseurs
	Body* GetBody();
	const Body* GetBody() const;

private:
	Body *mBody;
};