#pragma once
#include "Entity.h"

class LightEngine;
struct TextureHolder;
class PointLight : public Entity
{
public:
	// Ctor & dtor
	PointLight(int lightRadius = 256, sf::Color lightColor = sf::Color::White, int layer = 10);
	virtual ~PointLight(void);

	// Gestion de la position
	void SetPosition(const b2Vec2& pos);
	void SetPosition(const sf::Vector2f& pos);
	void Move(const b2Vec2& off);
	void Move(const sf::Vector2f& off);
	const b2Vec2& GetPosition(void) const;
	const sf::Vector2f& GetPosition_sf(void) const;

	// Mise � jour
	void Update(void);

	// Retourne la texture ombr�e
	const sf::Texture& GetShadowTexture(void) const;
	const sf::Sprite& GetShadowSprite(void) const;

	// G�re la couleur de la lumi�re
	sf::Color GetLightColor(void) const;
	void SetLightColor(const sf::Color& lightColor);

	// G�re la taille de la lampe
	unsigned int GetLightRadius(void) const;
	void SetLightRadius(unsigned int radius);

	// R�cup�re la bo�te englobante
	sf::FloatRect GetBoundingBox(void) const;

	// Accesseurs
	unsigned int GetID() const;

protected:
	// Acc�s du LightEngine
	friend class LightEngine;

	// RenderTexture
	sf::RenderTexture shadowTex;
	sf::Sprite shadowSprite;

	// Autres textures
	std::shared_ptr<TextureHolder> textures;

	// Vue
	sf::View view;

private:
	friend class LightManager;

	// Identifiant (uniques)
	unsigned int mID;

protected:
	// Cr�e les textures
	void CreateTextures(void);

	// Rendu
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// Propri�t�s
	bool mUpdate;
	
	// LightEngine
	LightEngine &mEngine;

	// Position
	sf::Vector2f mPosition;

	// Propri�t�s de la lumi�res
	sf::Color mLightColor;
	unsigned int mLightRadius;

	// Sprite de rendu
	sf::Sprite mRenderSprite;
};
