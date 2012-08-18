#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Thor/Resources.hpp>

class Body : public sf::Sprite
{
public:
	// Ctor & dtor
	Body(b2World *world, std::shared_ptr<sf::Texture> texture);
	virtual ~Body(void);

	// Crée le body dans le monde
	virtual void Create(b2Vec2 posInMeters) = 0;

	// Met à jour la position du sprite
	void Update();

	// Retourne true si l'objet est trop loin
	bool IsInRange(b2Vec2 const& xw, b2Vec2 const& yh) const;

	// Supprime le body du monde
	void Destroy();

	// Accesseurs
	bool IsStatic() const { return mBody->GetType() == b2_staticBody; }
	b2Body const* GetBody() const { return mBody; }
	b2World const* GetWorld() const { return mWorld; }
	b2Shape const* GetShape() const { return mShape; }

protected:
	// Variables pour l'objet physique
	b2World *mWorld;
	b2Body *mBody;
	b2Shape *mShape;

	// Variables pour l'objet dessinable
	std::shared_ptr<sf::Texture> mTexture;
};
