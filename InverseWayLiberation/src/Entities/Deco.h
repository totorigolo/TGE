#pragma once
#include "Entity.h"
#include "Hull.h"
#include "../Resources/Texture.h"

class Deco : public Entity
{
public:
	// Ctor & dtor
	Deco(int layer, Texture::Ptr texture, sf::Vector3f posRot); // PosRot pixel / degr�s
	virtual ~Deco();

	// Pour le rendu
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Texture
	Texture::Ptr& GetTexture(void);
	void SetTexture(Texture::Ptr texture);

	// Position et rotation
	void SetPosition(const b2Vec2 &pos);
	void SetPosition(const sf::Vector2f &pos);
	const b2Vec2 GetPosition() const;
	const sf::Vector2f GetPositionPx() const;
	void SetRotationD(float rot);
	void SetRotationR(float rot);
	const float GetRotationD() const;
	const float GetRotationR() const;

	// Accesseurs
	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;

private:
	sf::Sprite mSprite;
	Texture::Ptr mTexture;
};